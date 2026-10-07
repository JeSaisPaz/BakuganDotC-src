// bdc 0x08a32224 GmoMotionBaseSetName
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 3, set name) of the motion registry entry
   classes: empty. `GmoMotionRef` inherits it, since a reference entry keeps the name stored in
   the pack data (`ref->name` points into it). */

void GmoMotionBaseSetName(void *self, const char *name)

{
  return;
}

