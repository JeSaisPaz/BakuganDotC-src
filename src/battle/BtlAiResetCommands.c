// bdc 0x0888cfc8 BtlAiResetCommands
#include "bdc.h"

/* Resets all four command channels of `BtlAi` (`BtlAiChannelReset`), calls the
   Reset virtual (vtable entry 1, slot `+0xc`) of every non-NULL behaviour layer in `layerOrder`
   except the one at the active index, and clears the movement flags, the threat and incoming
   attack, the idle timer, the saved target and the odometer. */
void BtlAiResetCommands(BtlAi *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        BtlAiChannelReset(&self->channels[i]);
    }
    for (i = 0; i < 5; i++) {
        BtlAiLayer *layer;

        if (i == self->activeLayer) {
            continue;
        }
        layer = self->layerOrder[i];
        if (layer != NULL) {
            const VtblEntry *reset = &layer->vtbl[1];

            ((void (*)(void *))reset->fn)((u8 *)layer + reset->delta);
        }
    }
    self->moveFlags = 0;
    self->threat = NULL;
    self->incomingAttack = NULL;
    self->idleTime = 0.0f;
    self->savedTarget = NULL;
    self->odometer = 0.0f;
}
