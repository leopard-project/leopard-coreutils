#!/usr/bin/env python3
"""leopard-coreutils：基础用户态命令集（单仓、单包、多二进制）。"""
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parent
OUT  = ROOT / "out"

TOOLS = [
    "ls", "cat", "echo", "cp", "mv", "rm", "mkdir", "rmdir", "touch",
    "pwd", "sleep", "date", "uname", "whoami", "df", "free", "ps",
    "kill", "head", "tail", "wc", "grep", "true", "false", "basename",
    "dirname",
]

PKGNAME = "leopard-coreutils"
PKGVER  = "1.0_1"
ARCH    = "x86_64"
DESC    = "LeopardOS basic user commands"

# XACT CRT（../XACT-CRT）：Windows 风格启动对象 + 静态运行时 + 分配器
CRT_ROOT = ROOT / ".." / "XACT-CRT"
INC      = CRT_ROOT / "include"
LIB      = CRT_ROOT / "lib" / "x86_64"
CRT_OBJS = [LIB / "crt0.o", LIB / "start.o"]
CRT_LIBS = [LIB / "libxact.a", LIB / "liballoc.a"]

def rel(p):
    p = Path(p)
    if p.is_absolute():
        try:
            return str(p.relative_to(ROOT))
        except ValueError:
            return str(p)
    return str(p)


def esc(p):
    return rel(p).replace("$", "$$").replace(":", "$:").replace(" ", "$ ")


def main():
    cc  = os.environ.get("CC", "clang")
    cxx = os.environ.get("CXX", "clang++")
    ld  = os.environ.get("LD", "ld")
    opt = "-O" + os.environ.get("OPT_LEVEL", "2")
    col = os.environ.get("DIAGNOSTICS_COLOR", "-fdiagnostics-color=always")

    common = (f"{col} {opt} -Wall -g -ffreestanding -fno-builtin -m64 -mstackrealign "
              "-fno-stack-protector -fno-exceptions -fno-strict-aliasing -fshort-wchar -nostdinc")
    c_flags   = f"{common} -std=c11   -I {rel(INC)} -I . -MMD -MP"
    cpp_flags = f"{common} -std=c++11 -I {rel(INC)} -I . -Wno-write-strings -MMD -MP"

    L = ["# leopard-coreutils 构建 + xbps 打包", "ninja_required_version = 1.10",
         f"cc = {cc}", f"cxx = {cxx}", f"ld = {ld}",
         f"c_flags = {c_flags}", f"cpp_flags = {cpp_flags}", ""]
    L += ["rule cc",
          "  command = mkdir -p $$(dirname $out) && $cc $c_flags -MF $out.d -c $in -o $out",
          "  depfile = $out.d", "  deps = gcc", "",
          "rule cxx",
          "  command = mkdir -p $$(dirname $out) && $cxx $cpp_flags -MF $out.d -c $in -o $out",
          "  depfile = $out.d", "  deps = gcc", "",
          "rule link",
          "  command = $ld -Ttext=0x200000 $in -o $out", ""]

    core_obj = OUT / "lib" / "core.c.o"
    L += [f"build {esc(core_obj)}: cc {esc(ROOT / 'lib' / 'core.c')}", ""]

    elfs = []
    for tool in TOOLS:
        src = ROOT / tool / "main.c"
        obj = OUT / tool / "main.c.o"
        elf = OUT / f"{tool}.elf"
        L += [f"build {esc(obj)}: cc {esc(src)}", ""]
        ins = CRT_OBJS + [obj, core_obj] + CRT_LIBS
        L += [f"build {esc(elf)}: link {' '.join(esc(o) for o in ins)}", ""]
        L += [f"build build-{tool}: phony {esc(elf)}", ""]
        elfs.append(elf)

    pkg_out = OUT / f"{PKGNAME}-{PKGVER}.{ARCH}.xbps"
    args = f"-A {ARCH} -n {PKGNAME}-{PKGVER} -s \"{DESC}\""
    stage = ".pkg"
    cmd = f"rm -rf {stage}"
    for elf in elfs:
        dst = f"apps/system/{elf.name}"
        cmd += f" && mkdir -p {stage}/apps/system && cp {esc(elf)} {stage}/{dst}"
    cmd += f" && cd out && xbps-create --compression gzip {args} ../.pkg"

    L += ["rule package", f"  command = {cmd}", ""]
    L += [f"build {esc(pkg_out)}: package " + " ".join(esc(e) for e in elfs), ""]
    L += ["build build: phony " + " ".join(esc(e) for e in elfs), "default build"]
    L += ["build pack: phony " + esc(pkg_out), ""]
    L += ["rule clean", "  command = rm -rf out .pkg", "build clean: clean"]

    (ROOT / "build.ninja").write_text("\n".join(L) + "\n", encoding="utf-8")
    print("已生成 build.ninja（leopard-coreutils）。用法：ninja build / ninja pack")


if __name__ == "__main__":
    main()
