// bdc 0x08a32254 GmoMotionBaseGetArenaSize
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 9, get arena size) of the motion registry
   entry base class (vtable `0x08af6fe8`, see `GmoMotionBaseDtor`): returns 0. */

s32 GmoMotionBaseGetArenaSize(void *self)

{
  return 0;
}

