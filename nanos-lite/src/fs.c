#include "fs.h"

typedef struct
{
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
} Finfo;

enum
{
  FD_STDIN,
  FD_STDOUT,
  FD_STDERR,
  FD_FB,
  FD_EVENTS,
  FD_DISPINFO,
  FD_NORMAL
};

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
    {"stdin (note that this is not the actual stdin)", 0, 0, 0},
    {"stdout (note that this is not the actual stdout)", 0, 0, 0},
    {"stderr (note that this is not the actual stderr)", 0, 0, 0},
    [FD_FB] = {"/dev/fb", 0, 0, 0},
    [FD_EVENTS] = {"/dev/events", 0, 0, 0},
    [FD_DISPINFO] = {"/proc/dispinfo", 128, 0, 0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs()
{
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].disk_offset = _screen.width * _screen.height * sizeof(uint32_t);
}

int fs_open(const char *pathname, int flags, int mode)
{
  // Log("fs_open\n");
  for (int i = 0; i < NR_FILES; i++)
  {
    if (strcmp(file_table[i].name, pathname) == 0)
    {
      // Log("Open file %s successful:fd = %d\n", file_table[i].name, i);
      return i;
    }
  }
  panic("Open file %s failed:file not found\n", pathname);
  return -1;
}

ssize_t fs_read(int fd, void *buf, size_t len)
{
  // Log("fs_read\n");
  ssize_t ret = 0;
  switch (fd)
  {
  case FD_STDIN:
  case FD_STDOUT:
  case FD_STDERR:
    ret = 0;
    break;
  case FD_DISPINFO:
  {
    Finfo *file = &file_table[fd];
    size_t flen = dispinfo_read(buf, file->open_offset, len);
    file->open_offset += flen;
    break;
  }
  default:
  {
    Finfo *file = &file_table[fd];
    off_t foff = file->disk_offset + file->open_offset;
    int flen = len;
    if (len > (file->size - file->open_offset))
      flen = file->size - file->open_offset; // real file len
    assert(flen >= 0);
    ramdisk_read(buf, foff, flen);
    file->open_offset += flen;
    ret = flen;
    break;
  }
  }
  return ret;
}

ssize_t fs_write(int fd, const void *buf, size_t len)
{
  // Log("fs_write\n");
  ssize_t ret = 0;
  switch (fd)
  {
  case FD_STDIN:
    ret = 0;
    break;
  case FD_STDOUT:
  case FD_STDERR:
    for (int i = 0; i < len; i++)
    {
      _putc(((char *)buf)[i]);
    }
    ret = len;
    break;
  case FD_FB:

  default:
  {
    Finfo *file = &file_table[fd];
    off_t foff = file->disk_offset + file->open_offset;
    // Log("open_offset:%d\tlen:%d\n", file->open_offset, len);
    int flen = len;
    if (len > (file->size - file->open_offset))
      flen = file->size - file->open_offset; // real file len
    assert(flen >= 0);
    ramdisk_write(buf, foff, flen);
    file->open_offset += flen;
    // Log("done. open_offset:%d\tflen:%d\n", file->open_offset, flen);
    ret = flen;
    break;
  }
  }
  return ret;
}

off_t fs_lseek(int fd, off_t offset, int whence)
{
  // Log("fs_lseek\n");
  Finfo *file = &file_table[fd];
  switch (whence)
  {
  case SEEK_CUR:
    file->open_offset = offset + file->open_offset;
    break;
  case SEEK_END:
    file->open_offset = offset + file->size;
    break;
  case SEEK_SET:
    file->open_offset = offset;
    break;
  default:
    panic("Unvalid whence\n");
  }
  if (file->open_offset < 0 || file->open_offset > file->size)
  {
    panic("lseek offset out of bound:%d\n", file->open_offset);
  }
  // Log("file:%s\topen_offset:%d\n", file->name, file->open_offset);
  return file->open_offset;
}
int fs_close(int fd)
{
  // Log("fs_close\n");
  return 0;
}
size_t fs_filesz(int fd)
{
  Finfo *file = &file_table[fd];
  return file->size;
}