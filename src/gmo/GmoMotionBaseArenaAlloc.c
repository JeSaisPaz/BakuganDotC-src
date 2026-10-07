// bdc 0x08a32244 GmoMotionBaseArenaAlloc
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 7, arena alloc) of the motion registry
   entry classes: always returns NULL. `GmoMotionRef` inherits it, so a reference entry cannot
   allocate track data. */

void *GmoMotionBaseArenaAlloc(void *self, s32 size)

{
  return (void *)0x0;
}

