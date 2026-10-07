// bdc 0x08895cc8 BtlAiThinkLayerCheck
#include "bdc.h"

/* Check method of behaviour layer 4 of `BtlAi` (`MemberFnPtr` at `0x08a80320`):
   sets the layer's active byte and calls its virtual slot 2 (`+0x10`), so the think layer
   (`BtlAiThink`) is always eligible as the fallback. */
void BtlAiThinkLayerCheck(BtlAi *self)
{
    const VtblEntry *entry = &self->think.vtbl[2];

    self->think.active = 1;
    ((void (*)(void *))entry->fn)((u8 *)&self->think + entry->delta);
}
