// bdc 0x08863184 BtlBakuganQueueHit
#include "bdc.h"

/* Queues an extra hit on the unit for this frame: unless the queue is locked, full (8 entries) or
   the unit ignores hits (`stateFlags & 0x40000`), stores `attackType` and `attacker` in the next
   queue slot. `BtlBakuganApplyQueuedHits` resolves the queue. */
void BtlBakuganQueueHit(BtlBakugan *self, s16 attackType, void *attacker)
{
    s32 n;

    if (self->hitQueueLocked != 0) {
        return;
    }
    n = self->hitQueueCount;
    if (n >= 8 || (self->stateFlags & 0x40000) != 0) {
        return;
    }
    self->hitQueueType[n] = attackType;
    self->hitQueueAttacker[n] = attacker;
    self->hitQueueCount = n + 1;
}
