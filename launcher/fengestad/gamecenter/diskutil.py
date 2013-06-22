from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
from .nameutil import NameUtil


class DiskUtil(object):

    @classmethod
    def find_disks(cls, path, base_names=None, script=None):
        disks = [path]
        print("")
        print("FIRST DISK", path)
        print("")
        #disks = []
        use_name = script.rename_file(os.path.basename(path))
        number = NameUtil.find_number(use_name)
        first_without_number = NameUtil.strip_number(use_name)
        first_without_flags = NameUtil.strip_flags(first_without_number)
        if number is None:
            # only this disk
            return disks
        base_name = NameUtil.find_base_name(path)
        dir = os.path.dirname(path)
        candidates = {}
        items = []
        if base_names:
            items = base_names[base_name]
        else:
            all_items = os.listdir(dir)
            for item in all_items:
                b = NameUtil.find_base_name(item)
                if base_name == b:
                    items.append(item)
        #print("items is", items)
        for item in items:
            use_name = script.rename_file(item)
            if NameUtil.is_bad_dump(use_name):
                continue
            n = NameUtil.find_number(use_name)
            if n == 0:
                n = 99
            if n:
                candidates.setdefault(n, []).append((use_name,
                        os.path.join(dir, item)))
        #print(candidates)
        for n, items in candidates.iteritems():
            print("")
            print("FIND FLOPPIES FOR n", n, "=", items)
            print("")
            if n == 1:
                # already added first floppy
                continue
            print(" - floppy number", n)
            matches = []
            for use_name, p in items:
                without_number = NameUtil.strip_number(use_name)
                #print(without_number, "vs", first_without_number)
                if without_number == first_without_number:
                    print("perfect match")
                    #disks.append(p)
                    matches.append((0, p))
                    continue
                    #break
                without_flags = NameUtil.strip_flags(without_number)
                if without_flags == first_without_flags:
                    print("ok match")
                    if without_flags == without_number:
                        # there were no flags on this floppy
                        matches.append((1, p))
                    else:
                        # slightly worse score since there were flags
                        # not matching floppy 1
                        matches.append((2, p))
                    #disks.append(p)
                    #break
                #matches.append((2, p))
            if len(matches) == 0:
                #raise Exception("no candidates for floppy " + num)
                #print("WARNING: choosing partial matching floppy", p,
                #        "for floppy number", n)
                #disks.append(p)
                print("Did not find good match for floppy", n)
                print("candidates:")
                for item in items:
                    print("  ", item)
                raise Exception("Did not find good match for floppy {0}".format(n))
            matches.sort()
            print(matches)
            score, p = matches[0]
            #if score == 2:
            disks.append(p)
            # TOSEC (x of y) disk number labelling format
            if ' of {0})'.format(n) in path:
                # found the correct number of disks
                break
        return disks



