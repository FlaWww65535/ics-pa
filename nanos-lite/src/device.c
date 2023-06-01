#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
    [_KEY_NONE] = "NONE",
    _KEYS(NAME)};

size_t events_read(void *buf, size_t len)
{
  return 0;
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len)
{
  size_t flen = (offset + len > strlen(dispinfo)) ? (strlen(dispinfo) - offset) : len;
  memcpy(buf, dispinfo + offset, flen);
  return flen;
}

void fb_write(const void *buf, off_t offset, size_t len)
{
  int x = (offset / 4) % _screen.width;
  int y = (offset / 4) / _screen.width;
  int w = len / 4;
  int h = 1;
  _draw_rect((uint32_t *)buf, x, y, w, h);
  return len;
}

void init_device()
{
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d", _screen.width, _screen.height);
}
