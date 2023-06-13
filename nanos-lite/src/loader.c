#include "common.h"

#define DEFAULT_ENTRY ((void *)0x8048000)

uintptr_t loader(_Protect *as, const char *filename)
{
  Log("loader\n");
  size_t disk_size = get_ramdisk_size();
  int fd = fs_open(filename);
  fs_read(fd, DEFAULT_ENTRY, fs_filesz(fd));
  Log("file size %d\n", fs_filesz(fd));
  return (uintptr_t)DEFAULT_ENTRY;
}
