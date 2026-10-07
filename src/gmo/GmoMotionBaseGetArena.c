// bdc 0x08a3224c GmoMotionBaseGetArena
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 8, get arena) of the motion registry entry
   base class (vtable `0x08af6fe8`, see `GmoMotionBaseDtor`): returns NULL. */

u8 *GmoMotionBaseGetArena(void *self)

{
  return (u8 *)0x0;
}

