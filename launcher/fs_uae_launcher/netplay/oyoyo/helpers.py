# Copyright (c) 2008 Duncan Fordyce
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

""" contains helper functions for common irc commands """

import fs_uae_launcher.six as six
import random

def msg(cli, user, msg):
    for line in msg.split('\n'):
        cli.send("PRIVMSG", user, ":%s" % line)

def msgrandom(cli, choices, dest, user=None):
    o = "%s: " % user if user else ""
    o += random.choice(choices)
    msg(cli, dest, o)

def _makeMsgRandomFunc(choices):
    def func(cli, dest, user=None):
        msgrandom(cli, choices, dest, user)
    return func

msgYes = _makeMsgRandomFunc(['yes', 'alright', 'ok'])
msgOK = _makeMsgRandomFunc(['ok', 'done'])
msgNo = _makeMsgRandomFunc(['no', 'no-way'])


def ns(cli, *args):
    msg(cli, "NickServ", " ".join(args))

def cs(cli, *args):
    msg(cli, "ChanServ", " ".join(args))

def identify(cli, passwd, authuser="NickServ"):
    msg(cli, authuser, "IDENTIFY %s" % passwd)

def quit(cli, msg='gone'):
    cli.send("QUIT :%s" % msg)
    cli._end = 1

def user(cli, username, realname=None):
    cli.send("USER", username, cli.host, cli.host,
        realname or username)

_simple = (
    'join',
    'part',
    'nick',
    'notice',
)
def _addsimple():
    import sys
    def simplecmd(cmd_name):
        def f(cli, *args):
            cli.send(cmd_name, *args)
        return f
    m = sys.modules[__name__]
    for t in _simple:
        setattr(m, t, simplecmd(t.upper()))
_addsimple()

def _addNumerics():
    import sys
    from . import ircevents
    def numericcmd(cmd_num, cmd_name):
        def f(cli, *args):
            cli.send(cmd_num, *args)
        return f
    m = sys.modules[__name__]
    for num, name in six.iteritems(ircevents.numeric_events):
        setattr(m, name, numericcmd(num, name))

_addNumerics()

