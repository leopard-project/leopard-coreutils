#pragma once

#include <stdint.h>

const char *core_basename(const char *path);
int  core_is_dir(const char *path);
int  core_mkdir_p(const char *path);
int  core_copy_file(const char *src, const char *dst);
int  core_copy_tree(const char *src, const char *dst);
int  core_rm_rf(const char *path);
int  core_write_all(int fd, const void *buffer, size_t length);
