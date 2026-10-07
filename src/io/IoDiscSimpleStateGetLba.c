// bdc 0x089fa4e4 IoDiscSimpleStateGetLba
#include "bdc.h"

/* State 8 handler of the `CODiscSimple` disc reader (`g_discSimple`) (vtable slot `+0x4c`):
   `sceIoIoctl(fd, 0x01020006)` returns the file's start sector on the UMD, stored at `+0xfc`; then
   state 2 (get size). */

void IoDiscSimpleStateGetLba(IoDiscSimple *self)

{
  int ret;
  u32 lba;
  
  lba = 0;
  ret = sceIoIoctl(self->fd,0x1020006,(void *)0x0,0,&lba,4);
  self->result = ret;
  if (ret == 0) {
    self->lba = lba;
    self->state = 2;
  }
  return;
}

