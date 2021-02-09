#!/usr/bin/env python3
import os
import html
import shutil
import sys

if not os.path.exists("docs/scripts"):
    print("Run this script from the project root directory")
    sys.exit(1)

IGNORED = [".gitignore", "Makefile", "build.py", "update.py"]

option_names = set()

out_dir = "docs/html"

if os.path.exists(out_dir):
    shutil.rmtree(out_dir)
os.makedirs(out_dir)

for dir_path, dir_names, file_names in os.walk("docs/options"):
    if "dist" in dir_names:
        dir_names.remove("dist")
    if "html" in dir_names:
        dir_names.remove("html")
    if "internal" in dir_names:
        dir_names.remove("internal")
    if "scripts" in dir_names:
        dir_names.remove("scripts")
    if "todo" in dir_names:
        dir_names.remove("todo")
    for file_name in file_names:
        if file_name in IGNORED:
            continue
        if file_name.startswith("int_"):
            continue
        src = os.path.join(dir_path, file_name)
        assert src.startswith("docs/options")
        print(src)
        dst = os.path.join(out_dir, src[5:])
        dst = dst.replace("_", "-")
        dst = dst + ".html"
        print("=>", dst)
        if not os.path.exists(os.path.dirname(dst)):
            os.makedirs(os.path.dirname(dst))
        option_name = src.split("/", 3)[-1]
        option_path = src.split("/", 3)[-1]
        src2 = os.path.join("../doc/dist/", option_path)

        # Move launcher options up to base level in generated docs.
        dst = dst.replace("/launcher/", "/")
        option_path = option_name.replace("/launcher/", "/")

        if False and os.path.exists(src2):
            # CURRENTLY DISABLED
            shutil.copy(src2, dst)
        else:
            with open(src, "r") as f:
                data = f.read()
            data = data.split("Code:")[0]
            data = "".join(
                [
                    "<h1>{}</h1>\n".format(html.escape(src.split("/")[-1])),
                    "<pre>\n",
                    html.escape(data),
                    "</pre>\n",
                ]
            )
            with open(dst, "w") as f:
                f.write(data)
        option_names.add(option_path)

for dir_path, dir_names, file_names in os.walk("docs"):
    if "dist" in dir_names:
        dir_names.remove("dist")
    if "html" in dir_names:
        dir_names.remove("html")
    if "internal" in dir_names:
        dir_names.remove("internal")
    if "scripts" in dir_names:
        dir_names.remove("scripts")
    if "todo" in dir_names:
        dir_names.remove("todo")
    for file_name in file_names:
        if file_name in IGNORED:
            continue
        src = os.path.join(dir_path, file_name)
        assert src.startswith("docs/")
        print(src)
        dst = os.path.join(out_dir, src[5:])
        # dst = dst.replace("_", "-")

        if file_name.endswith(".md"):
            assert " " not in dst
            # assert "_" not in dst
            assert dst.endswith(".md")
            dst = dst[:-3] + ".html"
        print("=>", dst)
        if not os.path.exists(os.path.dirname(dst)):
            os.makedirs(os.path.dirname(dst))

        if file_name.endswith(".md"):
            # assert os.system("markdown {} > {}".format(src, dst)) == 0
            assert os.system("github-markup {} > {}".format(src, dst)) == 0
            # assert os.system("python3 -m markdown {} > {}".format(src, dst)) == 0
            with open(dst, "r", encoding="UTF-8") as f:
                data = f.read()
            data = data.replace('.md">', '">')
            with open(dst, "w", encoding="UTF-8") as f:
                f.write(data)
        elif "options" in src:
            pass
        else:
            shutil.copy(src, dst)

        if src.startswith("options/"):
            option_name = src.split("/")[-1]
            if option_name.endswith(".md"):
                option_name = option_name[:-3]
            option_name = option_name.replace("-", "_")
            option_names.add(option_name)


with open(os.path.join(out_dir, "options.html"), "w") as f:
    f.write("<h1>Configuration options</h1>\n")
    for option_name in sorted(option_names):
        if "/" in option_name:
            # Skip for now.
            continue
        f.write(
            '<a href="options/{}">{}</a><br>\n'.format(
                option_name.replace("_", "-",), option_name
            )
        )
