// bdc 0x08a323bc GmoMotionRefGetArena
#include "bdc.h"

/* `GmoMotionRef` virtual method (vtable `0x08af5424` entry 8, get arena): returns `ref->arena`
   (`+0x38`, the track data blob at `bin + 0x54`). */

u8 *GmoMotionRefGetArena(GmoMotionRef *ref)

{
  return ref->arena;
}

