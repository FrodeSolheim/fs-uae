#!/usr/bin/env python3

import os
import shutil
import sys

try:
    import markdown
except ImportError:
    markdown = None

if not os.path.exists("docs/scripts"):
    print("Run this script from the project root directory")
    sys.exit(1)

last_main_option = ""
last_main_option_added = False

option_data = {}
option_data_all = {}
options = {}
option_repl = {}

f = open("doc/options.html", "w", encoding="UTF-8")
if not os.path.exists("doc/dist"):
    os.makedirs("doc/dist")


def handle_option_data(name, data, option):
    global last_main_option, last_main_option_added
    since = ""
    see = ""
    default = ""
    example = ""
    example2 = ""
    option["type"] = ""
    text = []

    h_name = name.replace("-", "_")
    name = name.replace("_", "-")

    data = data.strip()
    org_lines = data.split("\n")
    lines = []
    in_code = False
    in_test = False
    for line in org_lines:
        if ":" in line or line.startswith("##"):
            if line.startswith("##"):
                key = line.strip("# ")
                value = ""
            else:
                key, value = line.split(":", 1)
            key = key.lower().strip()
            value = value.strip()
            if key == "since":
                since = value
                continue
            elif key == "see":
                see = value
                continue
            elif key == "default":
                default = value
                option["default"] = value
                continue
            elif key == "example":
                # example = value
                example2 = value
                continue
            elif key == "type":
                option["type"] = value
                continue
            elif key == "range":
                mi, ma = value.split(" - ")
                option["min"] = mi.strip()
                option["max"] = ma.strip()
                continue
            elif key == "description":
                if value.startswith('"'):
                    value = value.strip('"')
                    option["summary_translation"] = True
                else:
                    option["summary_translation"] = False
                option["summary"] = value
                continue
            elif key == "summary":
                if value.startswith('"'):
                    value = value.strip('"')
                    option["summary_translation"] = True
                else:
                    option["summary_translation"] = False
                option["summary"] = value
                continue
            elif key == "code":
                in_code = True
                continue
            elif key == "tests":
                in_code = False
            elif key.startswith("test "):
                in_code = False
                in_test = 1
            elif key == "input":
                in_code = False
                in_test = 1
            elif key == "output":
                in_test = 2
            elif key == "value":
                if " - " in value:
                    value, summary = value.split(" - ", 1)
                else:
                    summary = ""
                try:
                    n, desc = value.split("(", 1)
                except ValueError:
                    n, desc = value, value
                else:
                    n = n.strip()
                    desc = desc[:-1].strip()
                # FIXME: use summary
                option["values"].append((n, desc))
                # continue
        if in_code:
            option["code"].append(line[4:])
        elif in_test == 1:
            pass
        elif in_test == 2:
            pass
        else:
            lines.append(line)

    # text.append("\n<h2 id=\"{hname}\">"
    #             "{hname}<a name=\"{name}\"></a></h2>\n".format(
    #                 name=name, hname=h_name))
    text.append("<h1>{hname}</h1>\n".format(name=name, hname=h_name))
    if since:
        text.append("<p>")
        text.append("<i>Since {since}</i>.".format(since=since))
    if default:
        if since:
            text.append(" ")
        else:
            text.append("<p>")
        text.append("Default value: {default}".format(default=default))
    if example:
        if default:
            text.append(", ")
        elif since:
            text.append(" ")
        else:
            text.append("<p>")
        text.append("Example:")
    if since or default or example:
        text.append("</p>\n")

    if example2:
        text.append(
            "<pre>Example: {hname} = {value}</pre>\n".format(
                name=name, hname=h_name, value=example2
            )
        )

    in_list = False
    # in_para = False
    # in_value = False
    last_was_line = False
    has_started = False
    for line in lines:
        if not line.strip() and not has_started:
            continue
        has_started = True
        if (line.startswith("*") or line.startswith("Value:")) or (
            in_list and line.startswith(" ")
        ):
            started = False
            if not in_list:
                started = True
                text.append("<ul>\n")
                in_list = True
            if line.startswith("*") or line.startswith("Value:"):
                if not started:
                    text.append("</li>\n")
                text.append("<li>")
            else:
                text.append(" ")
            if line.startswith("Value:"):
                if "(" in line:
                    line = line.split("(")[0][6:].strip()
                else:
                    line = line[6:].strip()
                line += " - "
            else:
                line = line[1:].strip()
            text.append(line)
            last_was_line = False
        else:
            if in_list:
                text.append("</li>\n</ul>\n")
                in_list = False
            # if not line.strip():
            #    text.append("<p>")
            # else:
            if last_was_line:
                if text[-1] == "":
                    text.append("\n\n")
                else:
                    text.append(" ")
            if line.startswith("  "):
                text.append("<pre>{0}</pre>".format(line.strip()))
                last_was_line = False
            else:
                text.append(line)
                last_was_line = True
    if in_list:
        text.append("</li>\n</ul>\n")
        in_list = False
    if see:
        # text.append('\n<p>See <a href="#{see}">{see}</a></p>\n'.format(
        #     see=see))
        text.append(
            '\n<p>See <a href="{see_l}">{see}</a></p>\n'.format(
                see=see, see_l=see.replace("_", "-")
            )
        )

    t = "".join(text)
    t = t.replace("\n\n\n", "\n\n")
    while "  " in t:
        t = t.replace("  ", " ")
    t = t.replace("</pre><pre>", "\n")

    for key, value in option_repl.items():
        t = t.replace(key, value)

    if "</h2>\n\n<p>See" in t and last_main_option in t:
        if last_main_option_added:
            f.write(", ")
        else:
            f.write("\n\nSimilar options: ")
        f.write(
            '<a name="{name}"></a><a name="{hname}"></a>{hname}'.format(
                name=name, hname=h_name
            )
        )
        last_main_option_added = True
    else:
        last_main_option = name
        last_main_option_added = False
        f.write(t)
        # f.write("\n")


codes = {}


class Code:
    def __init__(self):
        self.dependencies = None
        self.marked = False


def handle_code(option, lines):
    # print("handle_code", option)
    inputs = set()
    for line in lines:
        line = line.replace("(", " ")
        line = line.replace(")", " ")
        line = line.replace(",", " ")
        line = line.replace(":", " ")
        words = line.split(" ")
        for word in words:
            word = word.strip()
            if not word:
                continue
            # if word[-1] in "):":
            #     word = word[:-1]
            if word.startswith("c."):
                name = word[2:]
                name = name.split(".")[0]
                if name != option:
                    inputs.add(name)
    inputs = sorted(inputs)
    code = Code()
    code.option = option
    code.inputs = sorted(inputs)
    code.lines = lines
    codes[option] = code

    print("   ", code.inputs)


def handle_option_file(name, path):
    print(name)
    if os.path.isfile(path):
        with open(path, "r", encoding="UTF-8") as option_f:
            original_name = name
            if original_name.endswith(".md"):
                name = name[:-3]
            option = {"values": [], "default": "", "code": [], "tests": []}
            option_text = option_f.read()
            handle_option_data(name, option_text, option)
            if option["code"]:
                handle_code(name, option["code"])
            if "summary" in option:
                option_data[name] = option
            option_data_all[name] = option
            if original_name.endswith(".md") and markdown is not None:
                if not os.path.exists("doc/html"):
                    os.makedirs("doc/html")
                html_path = "doc/html/" + name + ".html"
                with open(html_path, "w", encoding="UTF-8") as html_f:
                    html = markdown.markdown(option_text)
                    html_f.write(html)


def main():
    global f
    f.write("This page documents the options you can use in FS-UAE ")
    f.write('<a href="/fs-uae/configuration-files">configuration files</a>. ')
    f.write("The options are sorted in alphabetical order.")
    # f.write(" Both hyphens and ")
    # f.write("underscores can be used/mixed in option names.")
    f.write("\n")

    for name in os.listdir("docs/options"):
        if name == ".gitignore":
            continue
        if name.endswith("~"):
            continue
        if os.path.isfile(os.path.join("docs/options", name)):
            option_name = name
            if name.endswith(".md"):
                option_name, _ = os.path.splitext(name)
            options[option_name] = os.path.join("docs/options", name)
            # option_repl["[{0}]".format(name)] = "<a href=\"#{0}\">{0}</a>".format(name)
            option_repl["[{0}]".format(name)] = '<a href="{0}">{1}</a>'.format(
                name.replace("_", "-"), name
            )

    for name in os.listdir("docs/options/launcher"):
        if name.endswith("~"):
            continue
        if os.path.isfile(os.path.join("docs/options/launcher", name)):
            options[name] = os.path.join("docs/options", "launcher", name)
    for name in os.listdir("docs/options/arcade"):
        if name.endswith("~"):
            continue
        if os.path.isfile(os.path.join("docs/options/arcade", name)):
            options[name] = os.path.join("docs/options", "arcade", name)
    for name in os.listdir("docs/options/fsgs"):
        if name.endswith("~"):
            continue
        if os.path.isfile(os.path.join("docs/options/fsgs", name)):
            options[name] = os.path.join("docs/options", "fsgs", name)

    for name in sorted(options.keys()):
        if name == "Makefile":
            continue
        f = open("doc/dist/" + name, "w", encoding="UTF-8")
        handle_option_file(name, options[name])
        f.close()

    with open(
        "../fs-uae-launcher-private/fsgamesys/options/constants.py", "w", encoding="UTF-8"
    ) as f:
        f.write(
            """\
# Automatically generated - do not edit by hand
# noinspection SpellCheckingInspection

"""
        )
        for key in sorted(option_data_all.keys()):
            # Strip leading __ because that will invoke Python's
            # name mangling feature
            f.write('{} = "{}"\n'.format(key.upper().strip("__"), key))

    with open(
        "../fs-uae-launcher-private/fsgamesys/options/option.py", "w", encoding="UTF-8"
    ) as f:
        f.write(
            """\
# Automatically generated - do not edit by hand


# noinspection SpellCheckingInspection
class Option(object):
    \"\"\"Constants for option names.\"\"\"
"""
        )

        for key in sorted(option_data_all.keys()):
            # Strip leading __ because that will invoke Python's
            # name mangling feature
            f.write('    {} = "{}"\n'.format(key.upper().strip("__"), key))

    with open(
        "../fs-uae-launcher-private/launcher/option.py", "w", encoding="UTF-8"
    ) as f:
        f.write(
            """\
# Automatically generated - do not edit by hand

from fsgamesys.options.option import Option as BaseOption


# noinspection PyClassHasNoInit
class Option(BaseOption):
    @staticmethod
    def get(name):
        return options[name]


# noinspection PyPep8Naming
def N_(x):
    return x


options = {
"""
        )
        for key in sorted(option_data.keys()):
            print(key)
            option = option_data[key]
            f.write("    Option.{0}: {{\n".format(key.upper()))
            f.write('        "default": "{0}",\n'.format(option["default"]))
            if len(option["summary"]) == 0:
                f.write('        "description": "",\n')
            else:
                if key.startswith("uae_"):
                    f.write('        "description":')
                    if len(option["summary"]) < 50:
                        f.write(" ")
                else:
                    if option["summary_translation"]:
                        f.write('        "description": N_(')
                    else:
                        f.write('        "description": (')
                if len(option["summary"]) >= 50:
                    f.write("\n            ")
                if key.startswith("uae_"):
                    f.write('"{0}",\n'.format(option["summary"]))
                else:
                    f.write('"{0}"),\n'.format(option["summary"]))
            f.write('        "type": "{0}",\n'.format(option["type"]))
            if len(option["values"]) > 0:
                f.write('        "values": [\n')
                for name, desc in option["values"]:
                    if desc.startswith('"'):
                        if key.startswith("uae_"):
                            desc = "{0}".format(desc)
                        else:
                            desc = "N_({0})".format(desc)
                    else:
                        desc = '"{0}"'.format(desc)
                    f.write('            ("{0}", {1}),\n'.format(name, desc))
                f.write("        ]\n")
            if "min" in option:
                f.write('        "min": {0},\n'.format(option["min"]))
            if "max" in option:
                f.write('        "max": {0},\n'.format(option["max"]))
            f.write("    },\n")

        f.write("}\n")

    update_codes()


def update_code_dependencies(code):
    # print(sorted(codes.keys()))
    if code.dependencies is not None:
        return
    code.dependencies = set()
    code.dependencies.update(code.inputs)
    for dependency in code.inputs:
        code2 = codes[dependency]
        update_code_dependencies(code2)
        code.dependencies.update(code2.dependencies)


def write_option(f, option):
    code = codes[option]
    for dependency in sorted(code.dependencies):
        write_option(f, dependency)
    if not code.marked:
        code.marked = True
        f.write("    _{0}(c, f)\n".format(option))


def update_codes():
    for option, code in codes.items():
        update_code_dependencies(code)
    with open("doc/options2.py", "w") as f:
        f.write("# Automatically generated - do not edit by hand\n")
        f.write("\n")
        for option in sorted(codes.keys()):
            code = codes[option]
            f.write(
                "\n# noinspection PyUnusedLocal,"
                "SpellCheckingInspection,PyUnresolvedReferences\n"
            )
            f.write("def _{0}(c, f):\n".format(option))
            if option.startswith("int_"):
                f.write("    # noinspection PyUnresolvedReferences\n")
                f.write("    if c.{0}.explicit:\n".format(option))
                f.write(
                    '        f.fail("{0} was set explicitly")\n'.format(option)
                )
            uses_value = False
            for line in code.lines:
                if not line.strip():
                    continue
                if "value = " in line:
                    uses_value = True
                f.write("    {0}\n".format(line))
                if line.strip().startswith("f.fail("):
                    f.write(line.split("f.fail(")[0])
                    f.write('    raise Exception("Failed")\n')
            if uses_value:
                f.write("    c.{0} = value\n".format(option))
            f.write("\n")
        f.write(
            """\

class AbstractExpandFunctions:

    @staticmethod
    def matches(a, b):
        pass

    @staticmethod
    def fail(message):
        pass

    @staticmethod
    def warning(message):
        pass

    @staticmethod
    def lower(s):
        pass

"""
        )
        f.write("\ndef expand_config(c, f):\n")
        f.write("    assert isinstance(f, AbstractExpandFunctions)\n")
        for option in sorted(codes.keys()):
            write_option(f, option)
    shutil.move(
        "doc/options2.py", "../fs-uae-launcher-private/launcher/ui/config/expand.py"
    )


if __name__ == "__main__":
    main()
