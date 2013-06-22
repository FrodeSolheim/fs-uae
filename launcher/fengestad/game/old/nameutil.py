from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import re
import unicodedata
from .platform import GamePlatform


class GameNameUtil(object):

    @classmethod
    def create_fs_name(cls, name):
        name = name.replace(':', ' - ')
        name = name.replace('*', '-')
        name = name.replace('?', '')
        name = name.replace('/', '-')
        name = name.replace('"', "'")
        name = name.replace('>>>', " ")
        name = name.replace('<<<', " ")
        for i in range(3):
            name = name.replace('  ', ' ')
        while name.endswith('.'):
            name = name[:-1]
        name = name.strip()
        return name

    @classmethod
    def create_cmp_name(cls, name):
        name = unicode(name)
        decomposed = unicodedata.normalize('NFD', name)
        cmpname = []
        for c in decomposed.lower():
            if c in u'abcdefghijklmnopqrstuvwxyz0123456789+':
                cmpname.append(c)
        return u''.join(cmpname)

    @classmethod
    def create_cmpname(cls, name):
        return cls.create_cmp_name(name)

    @classmethod
    def is_bad_dump(cls, path):
        if '[b]' in path:
            return True
        m = re.search('\\[b [^\\]]*\\]', path, re.IGNORECASE)
        if m is not None:
            return True
        return False

    @classmethod
    def extract_platform(cls, path):
        p = path.upper()
        n = os.path.basename(p)
        for alias, platform in GamePlatform.aliases.iteritems():
            if '(' + alias + ',' in n:
                return platform
            if '(' + alias + ')' in n:
                return platform
        while os.path.dirname(p) != p:
            p = os.path.dirname(p)
            n = os.path.basename(p)
            for alias, platform in GamePlatform.aliases.iteritems():
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
        pos_list = []
        pos_list.append(name.find('('))
        pos_list.append(name.find('['))
        pos_list.append(name.rfind('.'))
        pos_list.append(len(name))
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
        game_name = ""
        config_name = ""
        name = os.path.basename(path)
        # find real case of file name
        #if os.path.dirname(path):
        #    for item in os.listdir(os.path.dirname(path)):
        #        if os.path.normcase(item) == os.path.normcase(name):
        #            name = item
        #            break
        name, ext = os.path.splitext(name)

        def ireplace(s, search, replace):
            index = s.lower().find(search.lower())
            if index >= 0:
                return s[:index] + replace + s[index+len(search):]
            return s

        name = cls.strip_number(name)
        #name = ireplace(name, '(save disk)', '')
        #name = ireplace(name, '(intro disk)', '')

        # TOSEC naming convention places vx.yy right after name,
        # without paranthesis - put paranthesis around version
        #print(name)
        name = re.sub('( v[0123456789][^ ]+) \\(', '(\\1) (', name)
        # also sometimes rXX where XX is the revision
        name = re.sub('( r[0123456789][^ ]+) \\(', '(\\1) (', name)

        # remove adjacent spaces
        name = re.sub('[ ]+', ' ', name)

        # find end of name (and start of config name)
        pos = name.find('(')
        pos2 = name.find('[')
        if pos > 0:
            if pos2 > 0 and pos2 < pos:
                pos = pos2
        if pos > 0:
            game_name = name[:pos].strip()
            config_name = name[pos:]
            config_name = config_name.replace('[!]', ' ')
            config_name = config_name.replace(',', ', ')
            config_name = config_name.replace(')', ', ')
            config_name = config_name.replace(']', ', ')
            config_name = config_name.replace('(', '')
            config_name = config_name.replace('[', '')
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


