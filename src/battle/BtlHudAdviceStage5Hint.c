// bdc 0x08837e94 BtlHudAdviceStage5Hint
#include "bdc.h"

/* Advice slot 0x17 (battle id 5): while the slot's advice state is below 2 and the player unit's
   `stage5HintFlag` is set, fires message 0x2da through `BtlHudAdviceStep`. */
void BtlHudAdviceStage5Hint(BtlHud *self, BtlBakugan *unit, int slot)
{
    if (self->adviceState[slot] < 2 && unit->stage5HintFlag != 0) {
        BtlHudAdviceStep(self, true, 0x2da, slot);
    }
}
