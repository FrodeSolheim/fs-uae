from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals
from itertools import combinations

import fs_uae_launcher.six as six
import os
import re
import unicodedata
import unittest
import itertools


class GameNameUtil(object):

    @classmethod
    def get_letter(cls, name):
        letter_name = name.upper()
        if letter_name.startswith("THE "):
            letter_name = letter_name[4:]
        if letter_name.startswith("A "):
            letter_name = letter_name[2:]
        for i in range(len(letter_name)):
            letter = letter_name[i]
            if letter in "01234567890":
                letter = "0"
                break
            if letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
                break
        else:
            raise Exception("could not find letter for game")
        return letter

    @classmethod
    def create_fs_name(cls, name):
        name = name.replace(':', ' - ')
        name = name.replace('*', '-')
        name = name.replace('?', '')
        name = name.replace('/', '-')
        name = name.replace('"', "'")
        name = name.replace('>>>', " ")
        name = name.replace('<<<', " ")
        # FIXME: regular expression instead
        for i in range(3):
            name = name.replace("  ", " ")
        while name.endswith('.'):
            name = name[:-1]
        name = name.strip()
        return name

    @classmethod
    def create_cmp_name(cls, name):
        name = six.text_type(name)
        decomposed = unicodedata.normalize('NFD', name)
        cmpname = []
        for c in decomposed.lower():
            if c in "abcdefghijklmnopqrstuvwxyz0123456789+":
                cmpname.append(c)
        return "".join(cmpname)

    @classmethod
    def create_cmpname(cls, name):
        return cls.create_cmp_name(name)

    @classmethod
    def is_bad_dump(cls, path):
        if '[b]' in path:
            return True
        m = re.search('\\[b[0-9]? [^\\]]*\\]', path, re.IGNORECASE)
        if m is not None:
            return True
        return False

    @classmethod
    def extract_platform(cls, path):
        p = path.upper()
        n = os.path.basename(p)
        for alias, platform in six.iteritems(GamePlatform.aliases):
            if '(' + alias + ',' in n:
                return platform
            if '(' + alias + ')' in n:
                return platform
        while os.path.dirname(p) != p:
            p = os.path.dirname(p)
            n = os.path.basename(p)
            for alias, platform in six.iteritems(GamePlatform.aliases):
                if '(' + alias + ',' in n:
                    return platform
                if '(' + alias + ')' in n:
                    return platform
            if n in GamePlatform.aliases:
                return GamePlatform.aliases[n]
        print("extract_platform returning None for", path)
        return None

    @classmethod
    def find_base_name(cls, path):
        name = os.path.basename(path)
        #name = os.path.normcase(name)
        pos_list = [name.find('('), name.find('['), name.rfind('.'), len(name)]
        for pos in sorted(pos_list):
            if pos > -1:
                return name[:pos].strip().lower()
        return name.strip().lower()

    @classmethod
    def find_alt(cls, path):
        m = re.search('\\[a([0-9]*)\\]', path, re.IGNORECASE)
        if m is not None:
            if not m.group(1):
                return 1
            return int(m.group(1))
        return None

    @classmethod
    def strip_alt(cls, path):
        path = re.sub('\\[a([0-9]*)\\]', '', path)
        return path

    @classmethod
    def find_disk_count(cls, name):
        for i in range(20):
            if "of {0})".format(i) in name:
                return i
        return 0

    @classmethod
    def find_number(cls, name):
        # find medium number, if any
        #number = None
        lname = name.lower()
        for prefix in 'reel', 'disk', 'cd':
            #print(prefix)
            pprefix = '(' + prefix + ' '
            index = lname.find(pprefix)
            #print(index)
            if index >= 0:
                rname = lname[index+len(pprefix):].strip()
                #print(rname)
                num = ''
                onlydigits = True
                for c in rname:
                    if c in '0123456789':
                        num += c
                    elif c in 'abcdefghijklmnopqrstuvwxyz':
                        onlydigits = False
                        num += c
                    else:
                        break
                if not num:
                    pass
                elif onlydigits:
                    #number = int(num)
                    return int(num)
                else:
                    num = num.lower()
                    if num == 'i':
                        return 1
                    elif num == 'ii':
                        return 2
                    elif num == 'iii':
                        return 3
                    elif num == 'iv':
                        return 4
                    elif num == 'v':
                        return 5
                    elif num == 'vi':
                        return 6
                    elif num == 'vii':
                        return 7
                    elif num == 'viii':
                        return 8
                    elif num == 'ix':
                        return 9
                    elif num == 'x':
                        return 10
                    elif len(num) == 1:
                        return 1 + ord(num) - ord('a')
                    else:
                        return num
                #print(' ----------- ', rname, '---', num, number)
        #return number

    @classmethod
    def strip_number(cls, name):
        # remove '(disk xxx)' from name
        # and also (disk xxx)(yyy)
        name = re.sub('\\([Dd][Ii][Ss][Kk][^)]*\\)(\\([^\\)]*\\))?', '', name)
        # remove 'disc x' from name
        name = re.sub('\\([Dd][Ii][Ss][Cc][^)]*\\)', '', name)
        # 'reel x' is used by Cinemaware games
        name = re.sub('\\([Rr][Ee][Ee][Ll][^)]*\\)', '', name)
        # remove 'cd x'
        name = re.sub('\\([Cc][Dd][^)]*\\)', '', name)
        # remove 'track x' from multi-part bin files
        name = re.sub('\\([Tt][Tr][Aa][Cc][Kk][^)]*\\)', '', name)
        # remove 'xxx disk' from name
        #name = re.sub('\\([^\\(]* [Dd][Ii][Ss][Kk]\\)', '', name)
        return name

    @classmethod
    def strip_flags(cls, name):
        name = re.sub('\\[[^\\]]*\\]', '', name)
        #name = name.replace('(Boot)', '')
        return name

    @classmethod
    def extract_names(cls, path, info={}):
        #game_name = ""
        config_name = ""
        name = os.path.basename(path)
        #print(name)
        # find real case of file name
        #if os.path.dirname(path):
        #    for item in os.listdir(os.path.dirname(path)):
        #        if os.path.normcase(item) == os.path.normcase(name):
        #            name = item
        #            break
        name, ext = os.path.splitext(name)

        # def ireplace(s, search, replace):
        #     index = s.lower().find(search.lower())
        #     if index >= 0:
        #         return s[:index] + replace + s[index+len(search):]
        #     return s

        name = cls.strip_number(name)
        #name = ireplace(name, '(save disk)', '')
        #name = ireplace(name, '(intro disk)', '')

        # TOSEC naming convention places vx.yy right after name,
        # without paranthesis - put paranthesis around version
        #print(name)
        name = re.sub('( v[0-9A-Z][^ ]*) \\(', '(\\1) (', name)
        # also sometimes rXX where XX is the revision
        name = re.sub('( r[0-9A-Z][^ ]*) \\(', '(\\1) (', name)
        #print("...", name)

        # remove adjacent spaces
        name = re.sub('[ ]+', ' ', name)

        name = name.replace("[", "(")
        name = name.replace("]", ")")

        parts = name.split("(")
        for i, part in enumerate(parts):
            if i == 0:
                continue
            if part[-1] != ")":
                continue
            commaed_parts = part.split(", ")
            if len(commaed_parts) == 2:
                parts[i] = commaed_parts[1][:-1] + " " + commaed_parts[0] + ")"
        name = "(".join(parts)

        # find end of name (and start of config name)
        pos = name.find('(')
        pos2 = name.find('[')
        if pos > 0:
            if 0 < pos2 < pos:
                pos = pos2
        if pos > 0:
            game_name = name[:pos].strip()
            config_name = name[pos:]
            config_name = config_name.replace('[!]', ' ')
            config_name = config_name.replace(',', ', ')
            config_name = config_name.replace(')', ', ')
            #config_name = config_name.replace(']', ', ')
            config_name = config_name.replace('(', '')
            #config_name = config_name.replace('[', '')
            config_name = config_name.replace(' ,', ',')
            config_name = config_name.strip(' ,')
            config_name = re.sub('[ ]+', ' ', config_name)
        else:
            game_name = name.strip()
        #if game_name.lower().endswith(', the'):
        #    game_name = 'The ' + game_name[:-5]
        #    game_name = game_name.strip()
        #elif game_name.lower().endswith(', a'):
        #    game_name = 'A ' + game_name[:-3]
        #    game_name = game_name.strip()
        return game_name, config_name

    @classmethod
    def extract_index_terms(cls, name, include_combinations=True):
        print("extract search terms for:", name)

        name = name.replace("\u00df", "ss")  # convert eszett

        words = name.lower().split(" ")

        new_words = []
        for word in words:
            #if word.endswith("n'"):
            #    print("- replacing n' with ng")
            #    word = word[:-2] + "ng"
            print("word:", word)
            letters = [" "]
            for letter in word:
                decomposed = unicodedata.normalize('NFD', letter)
                if len(decomposed) > 1:
                    for c in decomposed:
                        if c in "abcdefghijklmnopqrstuvwxyz0123456789+":
                            letters.append(c)
                else:
                    c = decomposed
                    # we want to include original punctuation, for now
                    #letters.append(c)
                    if c in "abcdefghijklmnopqrstuvwxyz0123456789+":
                        letters.append(c)
                    else:
                        # punctuation is replaced with blanks
                        if not letters[-1] == " ":
                            letters.append(" ")
            new_word = "".join(letters).strip()
            print("- new_word:", new_word)
            sub_words = new_word.split(" ")
            print("- sub_words:", sub_words)
            if include_combinations:
                combinations = [sub_words]
                if len(sub_words) > 0:
                    for i in range(1, len(sub_words)):
                        combinations.extend(
                            itertools.combinations(sub_words, i))
                print("- combinations:")
                for combination in combinations:
                    print(" ", combination)
                    new_words.append("".join(combination))
            else:
                new_words.extend(sub_words)
        print("new words:", new_words)

        terms = set()
        for word in new_words:
            try:
                add_words = normalize_words[word]
                print(" - replacement", word, "=>", add_words)
            except:
                add_words = [word]

            for add_word in add_words:
                if add_word.endswith("n"):
                    # leavin(') -> leaving
                    terms.add(add_word + "g")
                else:
                    terms.add(add_word)

        terms.difference_update(["a", "the", "and", "of", "for", "in"])
        print("terms", terms)
        return terms

    @classmethod
    def extract_search_terms(cls, name):
        return cls.extract_index_terms(name, include_combinations=False)

    # @classmethod
    # def extract_search_terms(cls, name):
    #     words = name.lower().split(" ")
    #
    #     new_words = []
    #     for word in words:
    #         #if word.endswith("n'"):
    #         #    print("- replacing n' with ng")
    #         #    word = word[:-2] + "ng"
    #         print("word:", word)
    #         letters = [" "]
    #         for letter in word:
    #             decomposed = unicodedata.normalize('NFD', letter)
    #             if len(decomposed) > 1:
    #                 for c in decomposed:
    #                     if c in "abcdefghijklmnopqrstuvwxyz0123456789+":
    #                         letters.append(c)
    #             else:
    #                 c = decomposed
    #                 # we want to include original punctuation, for now
    #                 #letters.append(c)
    #                 if c in "abcdefghijklmnopqrstuvwxyz0123456789+":
    #                     letters.append(c)
    #                 else:
    #                     # punctuation is replaced with blanks
    #                     if not letters[-1] == " ":
    #                         letters.append(" ")
    #         new_word = "".join(letters).strip()
    #         print("- new_word:", new_word)
    #         sub_words = new_word.split(" ")
    #         #print("- sub_words:", sub_words)
    #         #combinations = [sub_words]
    #         #if len(sub_words) > 0:
    #         #    for i in range(1, len(sub_words)):
    #         #        combinations.extend(itertools.combinations(sub_words, i))
    #         #print("- combinations:")
    #         #for combination in combinations:
    #         #    print(" ", combination)
    #         #    new_words.append("".join(combination))
    #     print("new words:", new_words)
    #
    #     terms = set()
    #     for word in new_words:
    #         try:
    #             add_words = normalize_words[word]
    #             print(" - replacement", word, "=>", add_words)
    #         except:
    #             add_words = [word]
    #
    #         for add_word in add_words:
    #             #terms.add(add_word)
    #             if add_word.endswith("n"):
    #                 # leavin(') -> leaving
    #                 terms.add(add_word + "g")
    #     print("terms", terms)
    #     return terms


normalize_words = {
    "se": ["special", "edition"],
    "one": ["1"],
    "two": ["2"],
    "three": ["3"],
    "four": ["4"],
    "five": ["5"],
    "six": ["6"],
    "seven": ["7"],
    "eight": ["8"],
    "nine": ["9"],
    "ten": ["10"],
    "eleven": ["11"],
    "twelve": ["12"],
    "thirteen": ["13"],
    "fourteen": ["14"],
    "fifteen": ["15"],
    "sixteen": ["16"],
    "seventeen": ["17"],
    "eighteen": ["18"],
    "nineteen": ["19"],
    "twenty": ["20"],
    "i": ["1"],
    "ii": ["2"],
    "iii": ["3"],
    "iv": ["4"],
    "v": ["5"],
    "vi": ["6"],
    "vii": ["7"],
    "viii": ["8"],
    "ix": ["9"],
    "x": ["10"],
    "xi": ["11"],
    "xii": ["12"],
    "n": ["and"],
    "versus": ["vs"],
    "brothers": ["bros"],
    "doctor": ["dr"],
    "them": ["em"],
    "mister": ["mr"],
    "miss": ["ms"],
    "television": ["tv"],
}
for i in range(1980, 2000):
    normalize_words[str(i - 1900)] = [str(i)]


class TestGameNameUtil(unittest.TestCase):

    def test_already_commaed_name(self):
        full_name = "Name (A, B, C).adf"
        name, variant = GameNameUtil.extract_names(full_name)
        self.assertEquals(name, "Name")
        self.assertEquals(variant, "A, B, C")

    def test_name_without_variant(self):
        full_name = "Name.adf"
        name, variant = GameNameUtil.extract_names(full_name)
        self.assertEquals(name, "Name")
        self.assertEquals(variant, "")

    def test_tosec_name(self):
        full_name = ("Lotus Turbo Challenge 2 (1991)(Gremlin)[cr CPY]"
                     "[t +4 Goonies].adf")
        name, variant = GameNameUtil.extract_names(full_name)
        self.assertEquals(name, "Lotus Turbo Challenge 2")
        self.assertEquals(variant, "1991, Gremlin, cr CPY, t +4 Goonies")

    def test_tosec_name_with_comma_in_publisher(self):
        full_name = "Zero Gravity (1997)(Hollemans, M.)(AGA).adf"
        name, variant = GameNameUtil.extract_names(full_name)
        self.assertEquals(name, "Zero Gravity")
        self.assertEquals(variant, "1997, M. Hollemans, AGA")

    def test_tosec_name_with_comma_in_publisher_2(self):
        full_name = "Ziriax (1990)(Software Business, The)[3001].dummy"
        name, variant = GameNameUtil.extract_names(full_name)
        self.assertEquals(name, "Ziriax")
        self.assertEquals(variant, "1990, The Software Business, 3001")

    def test_hitchhikers_guide_to_the_galaxy(self):
        full_name = "Leather Goddesses of Phobos r59 (1986)(Infocom).adf"
        name, variant = GameNameUtil.extract_names(full_name)
        self.assertEquals(name, "Leather Goddesses of Phobos")
        self.assertEquals(variant, "r59, 1986, Infocom")

    def test_leather_goddesses_of_phobos(self):
        full_name = ("Hitchhiker's Guide to the Galaxy, The r58 "
                     "(1986)(Infocom).adf")
        name, variant = GameNameUtil.extract_names(full_name)
        self.assertEquals(name, "Hitchhiker's Guide to the Galaxy, The")
        self.assertEquals(variant, "r58, 1986, Infocom")

    def test_extract_index_terms_kings_quest(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("King's Quest IV"),
            set(["king", "kings", "s", "quest", "4"]))

    def test_extract_index_terms_alien_breed_2(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("Alien Breed II"),
            set(["alieng", "breed", "2"]))

        self.assertEqual(
            GameNameUtil.extract_index_terms("Alien Breed 2"),
            set(["alieng", "breed", "2"]))

    def test_extract_index_terms_project_x_se_92(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("Project-X SE '92"),
            set(["projectx", "project", "10", "special",
                 "editiong", "1992"]))

    def test_extract_index_terms_leaving_teramis(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("Leavin' Teramis"),
            set(["leaving", "teramis"]))

    def test_extract_index_terms_fa_18_interceptor(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("F/A-18 Interceptor"),
            set(["f", "fa", "fa18", "f18", "a18", "f", "18", "a",
                 "interceptor"]))

    def test_extract_index_sensible_soccer_95_96(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("Sensible Soccer '95/'96"),
            set(["sensible", "soccer", "1995", "1996", "9596"]))

        self.assertEqual(
            GameNameUtil.extract_index_terms("Sensible Soccer 1995-96"),
            set(["sensible", "soccer", "1995", "1996", "199596"]))

    def test_extract_index_bump_n_burn(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("Bump 'n' Burn"),
            set(["bump", "and", "burng"]))

    def test_extract_index_bump_n_burn_2(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("Bump'n' Burn"),
            set(["bump", "and", "burng", "bumpng"]))

    def test_extract_index_bump_n_burn_3(self):
        self.assertEqual(
            GameNameUtil.extract_index_terms("Bump'n Burn"),
            set(["bump", "and", "burng", "bumpng"]))

    def test_search_bump_n_burn(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Bump 'n' Burn").issuperset(
                GameNameUtil.extract_search_terms("Bump'n Burn")))

    def test_search_bump_n_burn_2(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Bump 'n' Burn").issuperset(
                GameNameUtil.extract_search_terms("Bump and Burn")))

    def test_search_bump_n_burn_3(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Lotus Turbo Challenge 2")
            .issuperset(
                GameNameUtil.extract_search_terms("Lotus II")))

    def test_search_sensible_1(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Sensible Soccer '95/'96")
            .issuperset(
                GameNameUtil.extract_search_terms("soccer 95/96")))

    def test_search_fa_18(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("F/A-18 Interceptor")
            .issuperset(
                GameNameUtil.extract_search_terms("fa 18")))

    def test_search_formula_1(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Formula 1")
            .issuperset(
                GameNameUtil.extract_search_terms("formula one")))

    def test_search_formula_1_2(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Formula one")
            .issuperset(
                GameNameUtil.extract_search_terms("formula 1")))

    def test_search_projectx_1(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Project-X SE")
            .issuperset(
                GameNameUtil.extract_search_terms("project x special")))

    def test_search_projectx_2(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Project-X Special Edition")
            .issuperset(
                GameNameUtil.extract_search_terms("projectx se")))

    def test_search_yo_joe(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Yo! Joe!")
            .issuperset(
                GameNameUtil.extract_search_terms("yo joe")))

    def test_search_alien3(self):
        self.assertTrue(
            GameNameUtil.extract_index_terms("Alien 3")
            .issuperset(
                GameNameUtil.extract_search_terms("Alien\u00b3")))

if __name__ == "__main__":
    unittest.main()
