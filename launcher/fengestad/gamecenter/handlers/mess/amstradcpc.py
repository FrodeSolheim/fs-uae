from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mess import MessController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class AmstradCPCController(MessController):

    def configure_mess(self):
        self.args.append('cpc464')
        # FIXME: Check tyupe of file (floppy (.dsk), tape, cartridge..)
        path = self.context.game.file
        self.args.extend(['-flop1', path])

