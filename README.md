# leopard-coreutils

LeopardOS 的基础命令集，行为尽量贴近常用 coreutils。

## 命令

```
ls cat echo cp mv rm mkdir rmdir touch pwd sleep date uname whoami
df free ps kill head tail wc grep true false basename dirname
```

## 构建 / 打包

```bash
python3 gen_ninja.py
ninja build
ninja pack
```

安装位置：`/apps/system/<命令>.elf`；`lpsh` 会按 `path=` 查找。
