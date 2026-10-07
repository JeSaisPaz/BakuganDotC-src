// bdc 0x0888f208 BtlAiSetTarget
#include "bdc.h"

/* Sets the current target of the AI: for NULL clears the AI action words of the
   current and previous pad state and the owner's link (BtlBakuganClearLink),
   otherwise points the owner at the target (BtlBakuganSetTarget); then clears move
   flags 0x8000 and 0x10000 and marks the target as changed (BtlAiMarkTargetChanged). */
void BtlAiSetTarget(BtlAi *self, void *unit)
{
    BtlBakugan *owner;

    self->target = unit;
    owner = self->owner;
    if (unit == NULL) {
        self->pad.cur.aiActions = 0;
        self->pad.prev.aiActions = 0;
        BtlBakuganClearLink(owner);
    } else {
        BtlBakuganSetTarget(owner, self->target);
    }
    self->moveFlags &= ~0x18000u;
    BtlAiMarkTargetChanged(self);
}
