// bdc 0x0888d07c BtlAiResetAll
#include "bdc.h"

/* Full reset of `BtlAi`: `BtlAiResetCommands`, then also resets the active
   behaviour layer (layer vtable entry 1, skipped when the slot is empty) and makes layer 4 (the
   think layer, `BtlAiThink`) the active one. */
void BtlAiResetAll(BtlAi *self)
{
    BtlAiLayer *layer;

    BtlAiResetCommands(self);
    layer = self->layerOrder[self->activeLayer];
    if (layer != NULL) {
        const VtblEntry *reset = &layer->vtbl[1];

        ((void (*)(void *))reset->fn)((u8 *)layer + reset->delta);
    }
    self->activeLayer = 4;
}
