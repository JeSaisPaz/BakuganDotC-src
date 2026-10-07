// bdc 0x089db148 GmoDlCtxFinish
#include "bdc.h"

/* Closes a GMO display list: resets the texture offset/scale (`0x4a`/`0x4b` = 0, `0x48`/`0x49` =
   1.0) and the texture-map mode (`0xc0`), stores the end pointer in `*bufOut` and the remaining
   space in `*remain`, and returns the list length in bytes (0 if it overflowed). */

u32 GmoDlCtxFinish(GmoDlContext *self, void **bufOut, s32 *remain)

{
  u32 *p;
  u32 *pos;
  
  p = self->cur;
  self->cur = p + 1;
  *p = 0x4a000000;
  p = self->cur;
  self->cur = p + 1;
  *p = 0x4b000000;
  p = self->cur;
  self->cur = p + 1;
  *p = 0x483f8000;
  p = self->cur;
  self->cur = p + 1;
  *p = 0x493f8000;
  p = self->cur;
  self->cur = p + 1;
  *p = 0xc0000000;
  pos = self->cur;
  p = self->end;
  if (bufOut != NULL) {
    if (p < pos) {
      return 0;
    }
    *bufOut = pos;
  }
  if (remain != (s32 *)0x0) {
    *remain = (s32)((u8 *)p - (u8 *)pos);
  }
  return (u32)((u8 *)pos - (u8 *)self->start);
}

