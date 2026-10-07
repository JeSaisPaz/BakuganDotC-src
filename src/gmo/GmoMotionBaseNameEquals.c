// bdc 0x08a32234 GmoMotionBaseNameEquals
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 5, name equals) of the motion registry
   entry base class (vtable `0x08af6fe8`, see `GmoMotionBaseDtor`): returns false. */

bool GmoMotionBaseNameEquals(void *self, const char *name)

{
  return false;
}

