// bdc 0x08a0fb14 GmoImageSetAllocator
#include "bdc.h"

/* Sets the image library's default allocator pair for both slots (main `0x08af1240`/`0x08af124c`
   and VRAM `0x08af1244`/`0x08af1250`); NULL arguments select `malloc`/`free`. */

void GmoImageSetAllocator(void *allocFn, void *freeFn)

{
  if (allocFn == (void *)0x0) {
    allocFn = (void *)malloc;
  }
  if (freeFn == (void *)0x0) {
    freeFn = (void *)free;
  }
  g_gmoImageVramAlloc = allocFn;
  g_gmoImageVramFree = freeFn;
  g_gmoImageMainAlloc = allocFn;
  g_gmoImageMainFree = freeFn;
  return;
}
