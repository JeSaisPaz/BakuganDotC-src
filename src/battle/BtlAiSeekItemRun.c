// bdc 0x088958a4 BtlAiSeekItemRun
#include "bdc.h"

/* Run method of behaviour layer 2 of `BtlAi` (seek an item, `MemberFnPtr` at
   `0x08a80308`; check `BtlAiSeekItemCheck`): casts to the item's `pos`
   (`BtlAiRaycastToPoint`, which always returns 1, so in practice it always takes the first
   branch). When the cast returns non-zero, or the item is NULL, picked up (`pickedUp`) or claimed
   by a picker (`picker`), it clears the layer state (`seekItem` fields, `cmd.timerExpired` = 1,
   `item` = NULL); otherwise it sets the item's position as goal (`BtlAiSetGoalPoint`) unless
   the move channel has finished (`BtlAiChannelIsFinished`), then steps
   `BtlAiMoveToGoalPoint`. The cast address is formed from `item` before the NULL test. */
void BtlAiSeekItemRun(BtlAi *self)
{
    BtlAiSeekItemLayer *layer = &self->seekItem;
    BtlItem *item;

    if (BtlAiRaycastToPoint(self, ((BtlItem *)layer->item)->pos) == 0) {
        item = (BtlItem *)layer->item;
        if (item != NULL && item->pickedUp == 0 && item->picker == NULL) {
            if (BtlAiChannelIsFinished(self->channels) == 0) {
                BtlAiSetGoalPoint(self, ((BtlItem *)layer->item)->pos);
            }
            BtlAiMoveToGoalPoint(self);
            return;
        }
    }
    layer->base.state = 0;
    layer->base.active = 0;
    layer->base.flags = 0;
    layer->base.cmd.state = 0;
    layer->base.cmd.timerLimit = 0.0f;
    layer->base.cmd.timerElapsed = 0.0f;
    layer->base.cmd.timerExpired = 1;
    layer->base.cmd.flags = 0;
    layer->base.cmd.arg = 0;
    layer->base.cmd.argF = 0.0f;
    layer->base.cmd.finished = 0;
    layer->base.cmd.failed = 0;
    layer->item = NULL;
}
