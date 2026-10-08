// bdc 0x08a12eb4 GmoPlanTake
#include "bdc.h"

/* Carving pass of a model-library allocation plan (0x6c bytes, same layout as the image library's
   plan: 3 pool block pointers, 4 alignment-class totals per pool at `+0xc + pool * 0x10`, carve
   cursors at `+0x3c`): takes `size` bytes of `align`'s class from pool `pool`'s cursor, and adds a
   reference to the pool block. Returns the pointer, or 0 when `plan` is NULL, `size` is 0, the
   reservation is exhausted, or the cursor was 0. */

void *GmoPlanTake(void *plan, int pool, u32 align, int size)
{
  u32 *words = (u32 *)plan;
  u32 cls;
  u32 bytes;
  u32 remain;
  u32 rounded;
  u8 *ptr;
  GmoImageBlock *block;

  if (plan == NULL || size == 0) {
    return NULL;
  }
  if (align < 5) {
    cls = 3;
    bytes = 4;
  } else if (align < 0x11) {
    cls = 2;
    bytes = 0x10;
  } else if (align < 0x41) {
    cls = 1;
    bytes = 0x40;
  } else {
    cls = 0;
    bytes = 0x80;
  }
  remain = words[__builtin_offsetof(GmoImagePlan, totals) / sizeof(s32) + pool * 4 + cls];
  rounded = (bytes + size - 1) & -bytes;
  ptr = ((u8 **)words)[__builtin_offsetof(GmoImagePlan, cursors) / sizeof(u8 *) + pool * 4 + cls];
  if (remain < rounded) {
    return NULL;
  }
  ((u8 **)words)[__builtin_offsetof(GmoImagePlan, cursors) / sizeof(u8 *) + pool * 4 + cls] = ptr + rounded;
  words[__builtin_offsetof(GmoImagePlan, totals) / sizeof(s32) + pool * 4 + cls] = remain - rounded;
  if (ptr == NULL) {
    return NULL;
  }
  block = ((GmoImagePlan *)plan)->blocks[pool];
  if (block != NULL) {
    block->refCount++;
  }
  return ptr;
}
