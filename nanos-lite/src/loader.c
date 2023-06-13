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
    void *pg = new_page();
    void *va = DEFAULT_ENTRY + offset;
    _map(as, va, pg);
    size_t res = (fsize - offset);
    size_t len = res < PGSIZE ? res : PGSIZE;
    fs_read(fd, pg, len);
    printf("map page: paddr=%x vaddr=%x\n", pg, va);
  }
  Log("file size %d\n", fsize);
  return (uintptr_t)DEFAULT_ENTRY;
}
