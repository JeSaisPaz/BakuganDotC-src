// bdc 0x08a3225c GmoMotionBaseSetArena
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 10, set arena) of the motion registry
   entry classes: empty. `GmoMotionRef` inherits it; its arena pointer is fixed by
   `GmoMotionRefInitInPlace`. */

void GmoMotionBaseSetArena(void *self, u8 *arena)

{
  return;
}

