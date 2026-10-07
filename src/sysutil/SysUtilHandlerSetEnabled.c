// bdc 0x089cbdc8 SysUtilHandlerSetEnabled
#include "bdc.h"

/* Sets the handler's enabled byte (`+0xc`). Handlers are constructed disabled
   (`SysUtilHandlerCtor` passes 0). */

void SysUtilHandlerSetEnabled(SysUtilHandler *self, u8 enabled)

{
  self->enabled = enabled;
  return;
}

