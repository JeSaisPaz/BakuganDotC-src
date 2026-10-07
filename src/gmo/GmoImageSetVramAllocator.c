// bdc 0x08a0fb58 GmoImageSetVramAllocator
#include "bdc.h"

/* Sets only the second (VRAM) default allocator slot of the image library
   (`g_gmoImageVramAlloc`/`g_gmoImageVramFree`); NULL selects `malloc`/`free`. */

void GmoImageSetVramAllocator(void *alloc, void *free_fn)

{
  if (alloc == (void *)0x0) {
    alloc = (void *)malloc;
  }
  if (free_fn == (void *)0x0) {
    free_fn = (void *)free;
  }
  g_gmoImageVramAlloc = alloc;
  g_gmoImageVramFree = free_fn;
  return;
}
