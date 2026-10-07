// bdc 0x0881b5f4 NetPlayClearFlags
#include "bdc.h"

/* Clears the bits `mask` in the `NetPlay` flag word `+0xbc`. */

void NetPlayClearFlags(NetPlay *self, u32 mask)

{
  self->flags = self->flags & (mask ^ 0xffffffff);
  return;
}

