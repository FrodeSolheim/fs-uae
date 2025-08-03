#!/usr/bin/env python3

import csv
import os


def main():
    configurations_path = "od-fs/python/uae/configurations"
    csv_file = "Work/Options.csv"

    lines: list[str] = []
    for item in os.listdir(configurations_path):
        path = os.path.join(configurations_path, item)
        with open(path, "r") as f:
            lines.extend(f.readlines())

    names: list[str] = []
    for line in lines:
        line.strip()
        if not line or line.startswith('"') or line.startswith(";"):
            continue
        if '"""' in line:
            continue
        name = line.split("=", 1)[0].strip()
        names.append(name)

    rows: list[list[str]] = []
    with open(csv_file, "r") as f:
        reader = csv.reader(f)
        for row in reader:
            rows.append(row)

    if len(rows) > 0:
        header = rows.pop(0)
    else:
        header = ["OPTION", "FOO", "BAR"]

    existing_names: set[str] = set()
    for row in rows:
        if len(row) > 0 and row[0]:
            existing_names.add(row[0])

    for name in names:
        if name not in existing_names:
            rows.append([name])
            existing_names.add(name)

    rows.sort()
    rows.insert(0, header)

    # Force unix-style line endings for git and libreoffice compat
    with open(csv_file, "w", newline="\n") as f:
        writer = csv.writer(f)
        for row in rows:
            writer.writerow(row)


if __name__ == "__main__":
    main()
