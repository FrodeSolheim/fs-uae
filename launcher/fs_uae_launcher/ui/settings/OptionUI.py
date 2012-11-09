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
        group.layout.add(label, margin_right=20)
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

        elif option["type"] == "string":
            def on_change():
                value = text_field.get_text()
                Settings.set(name, value.strip())
            text_field = fsui.TextField(group)
            #text_field.set_min_width(400)
            text_field.set_text(Settings.get(name))
            text_field.on_change = on_change
            group.layout.add(text_field, expand=True)

        elif option["type"] == "integer" and "min" in option \
                and "max" in option:
            current = Settings.get(name)
            current_int = int(option["default"])
            if current:
                try:
                    current_int = int(current)
                except ValueError:
                    pass
            current_int = max(option["min"], min(option["max"], current_int))
            check_box = fsui.CheckBox(group, "Default")
            spin_ctrl = fsui.SpinCtrl(group, option["min"],
                    option["max"], current_int)
            if current == "":
                check_box.check()
                spin_ctrl.disable()
            def on_checkbox():
                if check_box.is_checked():
                    spin_ctrl.SetValue(int(option["default"]))
                    spin_ctrl.disable()
                    Settings.set(name, "")
                else:
                    spin_ctrl.enable()
            check_box.on_change = on_checkbox
            def on_spin():
                value = spin_ctrl.GetValue()
                value = max(option["min"], min(option["max"], value))
                Settings.set(name, str(value))
            spin_ctrl.on_change = on_spin
            group.layout.add_spacer(0, expand=True)
            group.layout.add(check_box)
            group.layout.add(spin_ctrl, margin_left=10)

        if choice_values:
            def on_change():
                index = choice.get_index()
                Settings.set(name, choice_values[index][0])
            choice_labels = [x[1] for x in choice_values]
            choice = fsui.Choice(group, choice_labels)
            current = Settings.get(name)
            for i, value in enumerate(choice_values):
                if current == value[0]:
                    choice.set_index(i)
                    break
            choice.on_change = on_change
            group.layout.add_spacer(0, expand=True)
            group.layout.add(choice)
        return group
