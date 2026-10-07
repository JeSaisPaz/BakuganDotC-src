// bdc 0x08a3223c GmoMotionBaseReserveArena
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 6, reserve arena) of the motion registry
   entry classes: empty. `GmoMotionRef` inherits it, its track data arena being part of the pack
   data. */

void GmoMotionBaseReserveArena(void *self, s32 size)

{
  return;
}

