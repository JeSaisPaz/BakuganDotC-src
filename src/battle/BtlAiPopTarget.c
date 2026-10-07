// bdc 0x0888f2c4 BtlAiPopTarget
#include "bdc.h"

/* Restores the target saved by `BtlAiPushTarget`: when `savedTarget` is set it is replaced by its
   `BtlBakuganListFind` result (NULL once the unit has left `g_btlBakuganList`); a unit still in
   the list becomes the target again (`BtlAiSetTarget`) and the slot is cleared. */

void BtlAiPopTarget(BtlAi *self)
{
    BtlBakugan *unit;

    if (self->savedTarget != NULL) {
        unit = BtlBakuganListFind(self->savedTarget);
        self->savedTarget = unit;
        if (unit != NULL) {
            BtlAiSetTarget(self, unit);
            self->savedTarget = NULL;
        }
    }
}
