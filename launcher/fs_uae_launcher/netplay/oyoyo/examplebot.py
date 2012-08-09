#!/usr/bin/python
"""Example bot for oyoyo that responds to !say"""

import logging
import re

from fs_uae_launcher.oyoyo.client import IRCClient
from fs_uae_launcher.oyoyo.cmdhandler import DefaultCommandHandler
from fs_uae_launcher.oyoyo import helpers


HOST = 'irc.freenode.net'
PORT = 6667
NICK = 'oyoyo-example'
CHANNEL = '#oyoyo-test'


class MyHandler(DefaultCommandHandler):
    def privmsg(self, nick, chan, msg):
        msg = msg.decode()
        match = re.match('\!say (.*)', msg)
        if match:
            to_say = match.group(1).strip()
            print('Saying, "%s"' % to_say)
            helpers.msg(self.client, chan, to_say)


def connect_cb(cli):
    helpers.join(cli, CHANNEL)


def main():
    logging.basicConfig(level=logging.DEBUG)

    cli = IRCClient(MyHandler, host=HOST, port=PORT, nick=NICK,
                    connect_cb=connect_cb)
    conn = cli.connect()

    while True:
        conn.next()      ## python 2
    # next(conn)     ## python 3


if __name__ == '__main__':
    main()
