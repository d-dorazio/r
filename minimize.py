#!/usr/bin/env python3


OPERATORS = r"=+-*/<>\{\};():?,!&[]^"


def tokens(src):
    i = 0

    def eat(ty, fun):
        nonlocal i
        tok = ""
        while i < len(src) and fun(src[i]):
            tok += src[i]
            i += 1
        return tok, ty

    while i < len(src):
        c = src[i]

        if c.isspace():
            eat("space", lambda c: c.isspace())
            continue

        elif c == "#":
            yield eat("macro", lambda c: c != "\n")
            i += 1

        elif c == '"':
            i += 1
            tok, ty = eat("str", lambda c: c != '"')
            yield f'"{tok}"', ty
            i += 1

        elif c.isdigit() or (c == "." and src[i + 1].isdigit()):
            yield eat("num", lambda c: c.isdigit() or c == ".")

        elif c == ".":
            yield c, c
            i += 1

        elif c.isalpha() or c == "_":
            yield eat("id", lambda c: src[i].isalnum() or src[i] == "_")

        elif c in OPERATORS:
            yield eat("op", lambda c: c in OPERATORS)

        else:
            assert False, c


def minimize(src):
    out = ""
    line = ""
    last_ty = None
    for tok, ty in tokens(src):
        if ty == "macro":
            if line:
                out += line + "\n"
            out += tok + "\n"
            line, last_ty = "", ""
            continue

        needs_space = (last_ty == "id" and ty == "num") or (
            last_ty == ty and ty != "op"
        )
        if len(line) + needs_space + len(tok) > 80:
            out += line + "\n"
            line, last_ty = tok, ty
        else:
            if needs_space:
                line += " "
            line += tok
            last_ty = ty

    out += line

    return out


def main(src, dst):
    with open(src) as fp:
        c_code = fp.read()

    minimized = minimize(c_code)

    with open(dst, "wt") as fp:
        fp.write(minimized)


if __name__ == "__main__":
    main("main.cpp", "r.cpp")
