// bdc 0x08a32394 GmoMotionRefGetName
#include "bdc.h"

/* `GmoMotionRef` virtual method (vtable `0x08af5424` entry 4, get name): returns `ref->name`
   (`+0x30`, pointing at `bin + 0x30`). */

const char *GmoMotionRefGetName(GmoMotionRef *ref)

{
  return ref->name;
}

