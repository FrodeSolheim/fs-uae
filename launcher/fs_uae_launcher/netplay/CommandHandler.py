from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from .oyoyo.client import IRCClient
from .oyoyo.cmdhandler import DefaultCommandHandler
from .oyoyo import helpers

class CommandHandler(DefaultCommandHandler):

    #def __init__(self, client, handler):
    #    DefaultCommandHandler.__init__(self, client)

    #def privmsg(self, nick, chan, msg):
    #    print("%s in %s said: %s" % (nick, chan, msg))
    #
    def nicknameinuse(self, nick, *message):
        print("Nickname in use", nick, message)
        helpers.nick(self.client, self.client.handler.generate_nick())

    #def endofmotd(self, *args):
    #    print("end of motd", args)
    #    helpers.join(self.client, "#FS-UAE")

    #def __getattr__(self, name):
    #    print("__getattr__", name)
    #    def handler(*args):
    #        print(name, args)
    #    return handler
    def __unhandled__(self, command, *args):
    #    print("__unhandled__", command, args)
        handler = self.client.handler
        #handler.queue_message((command, args))
        handler.post_message(command, args)
