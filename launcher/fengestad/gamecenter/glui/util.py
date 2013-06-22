from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

def sort_configurations(items):
    # Cracked games score better, since these need no
    # manual lookup, etc.
    score_table = [
        [' AGA ', -(2**16)], # amiga AGA version
        [' cr ', -(2**15)], # cracked version
        [' NTSC ', -(2**14)], # prefer NTSC version
    ]
    scored = []
    for item in items:
        check = ' ' + item.replace(',', ' ') + ' '
        score = 0
        for p, s in score_table:
            if p in check:
                #scored.append((0, item))
                score += s
        # prefer shortest configs, all else being equal
        score += len(item)
        scored.append((score, item))
        #scored.append(1, item)
    scored.sort()
    return [x[1] for x in scored]
