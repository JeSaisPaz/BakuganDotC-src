// bdc 0x089cdedc PadBaseCtor
#include "bdc.h"

/* Base-class constructor of the controller object: installs the base vtable `0x08af532c` at
   `+0x4c`, sets `initArg` (`+0x38`) to 0, **`enabled` (`+0x3a`) to 1**, `readOk` (`+0x3b`) to 0 and
   clears the buffer pointer `+0x40` and `+0x44`. Returns `pad`. */

PadState *PadBaseCtor(PadState *pad)

{
  pad->vtable = &g_padBaseVtable;
  pad->initArg = '\0';
  pad->enabled = '\x01';
  pad->readOk = '\0';
  pad->buffer = (void *)0x0;
  pad->unk44 = 0;
  return pad;
}

