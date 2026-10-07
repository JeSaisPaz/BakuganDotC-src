// bdc 0x08a3238c GmoMotionRefGetInfo
#include "bdc.h"

/* `GmoMotionRef` virtual method (vtable `0x08af5424` entry 2, get info): returns `ref->info`
   (`+0x2c`), the in-place `GmoMotionInfo` at the start of the pack data. */

void *GmoMotionRefGetInfo(GmoMotionRef *ref)

{
  return ref->info;
}

