#!/usr/bin/env python3

import re
import subprocess
import textwrap


def minimize(src):
    out = ""
    body = ""

    for l in src.splitlines():
        if l.startswith("#define"):
            out += l.strip() + "\n"
            continue
        if l.startswith("#include"):
            out += l.strip().replace(" ", "") + "\n"
            continue

        l = l.replace("\t", "")
        l = l.replace("explicit", "")
        l = re.sub(r" +", " ", l)

        for s in r"=+-*/<>\{\};():?,":
            l = l.replace(" {}".format(s), s).replace("{} ".format(s), s)

        if '"' in l:
            out += "\n".join(textwrap.wrap(body, 80))
            out += "\n" + l
            body = ""
        else:
            body += l

    return out + "\n".join(textwrap.wrap(body, 80))


def main(src, dst):
    with open(src) as fp:
        c_code = fp.read()

    minimized = minimize(c_code)

    with open(dst, "wt") as fp:
        fp.write(minimized)


if __name__ == "__main__":
    main("main.cpp", "r.cpp")
