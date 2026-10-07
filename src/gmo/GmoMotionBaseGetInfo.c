// bdc 0x08a3221c GmoMotionBaseGetInfo
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 2, get info) of the motion registry entry
   base class (vtable `0x08af6fe8`, see `GmoMotionBaseDtor`): returns NULL. */

GmoMotionInfo *GmoMotionBaseGetInfo(void *self)

{
  return (GmoMotionInfo *)0x0;
}

