# leopard-coreutils

基础用户态命令集（ls/cat/cp/rm/…）。一个仓库、一个 xbps 包、多个静态二进制，
安装到 `/apps/system/<cmd>.elf`，由 lpsh 通过 `path=/apps/system:...` 解析。

## 构建

    python3 gen_ninja.py && ninja build   # → out/<cmd>.elf
    ninja pack                            # → out/leopard-coreutils-*.xbps

- 依赖 `../leopard-sdk/out/crt`（官方运行时 + `start.cpp` 入口）。
- `lib/core.c` 提供递归复制/删除、mkdir -p 等公共实现。
- 只使用 v5 SDK 的 POSIX 封装（open/read/write/opendir/stat/…）。

## 约定

- 每个命令一个目录，入口 `main.c`。
- 输出用 `printf`/`fputc`，不要用不存在的 `puts`/`putchar`。
- 新增命令：加目录、加 `main.c`、在 `gen_ninja.py` 的 `TOOLS` 里加名字。
- 相对路径依赖调用者 cwd；脚本里请配合 shell 的 `cd`（lpsh 已同步 chdir）。
