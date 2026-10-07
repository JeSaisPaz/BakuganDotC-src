// bdc 0x08a2a468 BtlAiLayerIsActive
#include "bdc.h"

/* Entry 2 (fn at `+0x14`) of every CPU AI behaviour layer vtable: returns the layer's active byte
   `+0x14`. */

u8 BtlAiLayerIsActive(BtlAiLayer *self)
{
  return self->active;
}
