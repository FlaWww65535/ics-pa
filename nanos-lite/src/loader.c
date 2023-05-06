#include "common.h"

#define DEFAULT_ENTRY ((void *)0x4000000)

uintptr_t loader(_Protect *as, const char *filename) {
  size_t disk_size = get_ramdisk_size();
  ramdisk_read(DEFAULT_ENTRY,0,disk_size);
  printf("test");
  return (uintptr_t)DEFAULT_ENTRY;
}
