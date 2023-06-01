#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
    [_KEY_NONE] = "NONE",
    _KEYS(NAME)};

#define KEYDOWN_MASK 0x8000

size_t events_read(void *buf, size_t len)
{
  printf("events_read len = %d\n", len);
  char *eve[20];
  int key = _read_key();
  if (key == _KEY_NONE)
  {
    unsigned long time = _uptime();
    sprintf(eve, "t %d\n", time);
  }
  else
  {
    if (key & KEYDOWN_MASK)
    {
      sprintf(eve, "kd %d\n", keyname[key & 0xfff]);
    }
    else
    {
      sprintf(eve, "ku %d\n", keyname[key & 0xfff]);
    }
  }
  memcpy(buf, eve, len);
  printf("buf is %s\n", buf);
  return strlen(buf);
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, off_t offset, size_t len)
{

  size_t flen = (offset + len > strlen(dispinfo)) ? (strlen(dispinfo) - offset) : len;
  // printf("dispinfo_read: offset = %d,len = %d,flen = %d\n", offset, len, flen);
  memcpy(buf, dispinfo + offset, flen);
  return flen;
}

size_t fb_write(const void *buf, off_t offset, size_t len)
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
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", _screen.width, _screen.height);
}
