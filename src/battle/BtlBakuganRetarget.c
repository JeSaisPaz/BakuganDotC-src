// bdc 0x08863030 BtlBakuganRetarget
#include "bdc.h"

/* Re-picks the Bakugan's target with BtlBakuganPickTarget, repeating while
   at least two candidates remain and the new pick resolves to the same object
   as the old target. When the target id changed, sets retargeted (only if the
   new id is nonzero) and adds 1 to stats counter 0x10. */
void BtlBakuganRetarget(BtlBakugan *self, s32 mode)
{
    void *oldTarget = BtlBakuganGetTarget(self);
    u32 oldId = self->targetId;
    void *newTarget;

    do {
        self->targetId = BtlBakuganPickTarget(self, mode);
        newTarget = BtlBakuganGetTarget(self);
        if (BtlBakuganCountTargetCandidates(self, mode) < 2) {
            break;
        }
    } while (oldTarget == newTarget);
    if (oldId != self->targetId) {
        if (self->targetId != 0) {
            self->retargeted = 1;
        }
        if (self->stats != NULL) {
            BtlStatsAddCounter(self->stats, 0x10, 1);
        }
    }
}
