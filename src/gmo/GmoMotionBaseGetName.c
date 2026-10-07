// bdc 0x08a3222c GmoMotionBaseGetName
#include "bdc.h"

/* Default implementation (base vtable `0x08af6fe8` entry 4, get name) of the motion registry entry
   base class (vtable `0x08af6fe8`, see `GmoMotionBaseDtor`): returns NULL. */

const char *GmoMotionBaseGetName(void *self)

{
  return (char *)0x0;
}

