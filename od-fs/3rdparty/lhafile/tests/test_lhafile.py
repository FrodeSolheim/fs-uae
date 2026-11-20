# -*- coding:utf-8 -*-
# Copyright (c) 2010 Hidekazu Ohnishi.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
#    * Neither the name of the author nor the names of its contributors
#      may be used to endorse or promote products derived from this
#      software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""Lhafile, extension extract lzh file.

This is unittest code for lhafile.
"""
try:
    from cStringIO import StringIO
except ImportError:
    from io import StringIO
import os
import os.path
import random
import unittest

import lhafile

class TestSequenceFunctions(unittest.TestCase):
    
    def make_testfile(self, chars, length, words, filesize):
        fout = StringIO()
        randint = random.randint
        # make random strings
        source = []
        for i in range(words):
            w = ""
            for j in range(randint(1, length)):
                c = randint(1, chars)
                w += chr(c)
            source.append(w)
        # make test file from random strings
        size = 0
        while size < filesize:
            s = source[randint(0, len(source)-1)]
            l = min(randint(1, max(len(s)-1, 1)), filesize - size)
            fout.write(s[:l])
            size += l
        return fout.getvalue()
    
    def setUp(self):
        random.seed(32264)        
        self.lzhnames = ['archive_lhaca.lzh', 'archive_lhaplus.lzh', 'archive_lhaunix_lh5.lzh', 'archive_lhaunix_lh6.lzh', 'archive_lhaunix_lh7.lzh']
        self.datasets = {
            'archive/01_simple/01_test.txt':"a",            
            'archive/01_simple/02_test.txt':"abcabcaabcdefg",
            'archive/01_simple/03_test.txt':"abbcccddddeeeeebccddd",            
            'archive/01_simple/11_data.bin':self.make_testfile(1, 1, 1, 4),
            'archive/01_simple/12_data.bin':self.make_testfile(1, 1, 1, 10),
            'archive/01_simple/13_data.bin':self.make_testfile(5, 200, 10, 16*1024-1),
            'archive/01_simple/14_data.bin':self.make_testfile(2, 200, 10, 16*1024-1),
            'archive/01_simple/15_data.bin':self.make_testfile(5, 200, 10, 16*1024+1),
            'archive/01_simple/16_data.bin':self.make_testfile(5, 200, 10, 64*1024),
            'archive/02_complex/21_data.bin':self.make_testfile(255, 200, 100, 65535),
            'archive/02_complex/22_data.bin':self.make_testfile(255, 200, 100, 65536),
            }
        #for path in self.datasets.keys():
        #    try:
        #        os.makedirs(os.path.dirname(path))
        #    except:
        #        pass
        #    open(path, "wb").write(self.datasets[path])
        
    def testlhafile_infolist(self):
        for lzhname in self.lzhnames:
            lha = lhafile.Lhafile(lzhname)
            files = [info.filename for info in lha.infolist()]
            for filename in self.datasets.keys():
                filename = os.sep.join(filename.split('/'))
                self.assert_(filename in files)
                del files[files.index(filename)]
      
                
    def testlhafile_read(self):
        for lzhname in self.lzhnames:
            lha = lhafile.Lhafile(lzhname)
            files = [info.filename for info in lha.infolist()]
            datafiles = self.datasets.keys()
            datafiles.sort()
            for filename in datafiles:
                try:
                    norm_filename = os.sep.join(filename.split('/'))                    
                    data = lha.read(norm_filename)
                except Exception as e:
                    self.assert_(False, "Decode error happened in %s" % (filename,))
                if data == self.datasets[filename]:
                    continue
                if len(data) != len(self.datasets[filename]):
                    self.assert_(False, "Data length is not matched %s" % (filename,))
                elif data != self.datasets[filename]:
                    i = 0
                    while True:
                        if data[i] != self.datasets[filename][i]:
                            print (data[max(0,i-10):i+10],)
                            print (self.datasets[filename][max(0,i-10):i+10],)
                            self.assert_(False, "Data mismatched in %s, %d" % (filename,i ))
                        i += 1

if __name__ == '__main__':
    unittest.main()
