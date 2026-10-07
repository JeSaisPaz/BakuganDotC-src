// bdc 0x0888ddb0 BtlAiSetOwnerId
#include "bdc.h"

/* Owner-id setup of `BtlAi` (from `BtlAiCreate` with `unit+0x150`): stores `id` in
   `ownerId`, sets bits 0..29 of `allowedCmds` (all 30 commands allowed, other bits kept), fills
   `layerOrder` with the five embedded layers in priority order (guard, follow, seek-item, wander,
   think) and refreshes the rule mode (`BtlAiUpdateRuleMode`). */
void BtlAiSetOwnerId(BtlAi *self, s32 id)
{
    u32 allowed;
    s32 i;

    self->ownerId = id;
    allowed = self->allowedCmds;
    for (i = 0; i < 30; i++) {
        allowed |= 1u << i;
    }
    self->allowedCmds = allowed;
    self->layerOrder[0] = (BtlAiLayer *)self;
    self->layerOrder[1] = &self->follow.base;
    self->layerOrder[2] = &self->seekItem.base;
    self->layerOrder[3] = &self->wander.base;
    self->layerOrder[4] = &self->think;
    BtlAiUpdateRuleMode(self);
}
