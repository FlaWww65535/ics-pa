#include "common.h"
#include "memory.h"

#define DEFAULT_ENTRY ((void *)0x8048000)

uintptr_t loader(_Protect *as, const char *filename)
{
  Log("loader\n");
  size_t disk_size = get_ramdisk_size();
  int fd = fs_open(filename);
  size_t fsize = fs_filesz(fd);
  for (int offset = 0; offset < fsize; offset += PGSIZE)
  {
    void *pg = 0; // newpage();
    void *va = DEFAULT_ENTRY + offset;
    _map(as, va, pg);
    size_t res = (fsize - offset);
    fs_read(fd, pg, res < PGSIZE ? res : PGSIZE);
  }
  Log("file size %d\n", fsize);
  return (uintptr_t)DEFAULT_ENTRY;
}
