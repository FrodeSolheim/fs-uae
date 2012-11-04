from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from ...Settings import Settings
from ...Signal import Signal
from ...Options import Options

class OptionUI:

    @classmethod
    def create_group(cls, parent, name):
        group = fsui.Group(parent)
        group.layout = fsui.HorizontalLayout()
        option = Options.get(name)
        label = fsui.Label(group, _(option["description"]))
        group.layout.add(label, expand=True)

        choice_values = []
        if option["type"] == "boolean":
            if option["default"] == "1":
                default_desc = _("Default ({0})").format(_("On"))
            elif option["default"] == "0":
                default_desc = _("Default ({0})").format(_("Off"))
            else:
                default_desc = _("Default")
            choice_values.append(("", default_desc))
            choice_values.append(("1", _("On")))
            choice_values.append(("0", _("Off")))
        elif option["type"] == "choice":
            for i, value in enumerate(option["values"]):
                if option["default"] == value[0]:
                    default_desc = _("Default ({0})").format(_(value[1]))
                    break
            else:
                default_desc = _("Default")
            choice_values.append(("", default_desc))
            for option in option["values"]:
                choice_values.append((option[0], _(option[1])))

        if choice_values:
            def on_choice_change():
                index = choice.get_index()
                Settings.set(name, choice_values[index][0])
            choice_labels = [x[1] for x in choice_values]
            choice = fsui.Choice(group, choice_labels)
            current = Settings.get(name)
            for i, value in enumerate(choice_values):
                if current == value[0]:
                    choice.set_index(i)
                    break
            choice.on_change = on_choice_change
            group.layout.add(choice)
        return group
