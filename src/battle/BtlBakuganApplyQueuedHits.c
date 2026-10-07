// bdc 0x08865ee4 BtlBakuganApplyQueuedHits
#include "bdc.h"

/* Resolves the extra hits queued on a unit during the frame (`hitQueueCount` entries of
   `hitQueueType` / `hitQueueAttacker`). For each attacker still in the battle list
   (`BtlBakuganListFind`) that matches `lockedAttacker` (or any attacker when it is NULL), adds
   `flinchPower × hitCount` to `*flinch` and `knockdownPower × hitCount` to `*knockdown`
   (`BtlAttackParamsGetFlinchPower`, `BtlAttackParamsGetKnockdownPower`,
   `BtlAttackParamsGetHitCount`), credits the hit count to the attacker's combo
   (`BtlBakuganAddComboHits`) when this unit answers virtual slot 10 or 12, stores this unit in
   the attacker's `linkedUnit`, and applies the hit with `BtlCombatTakeHit` (hit class 2, attack
   id `type + 0x23`, attribute 6). Then clears the queue count. */
void BtlBakuganApplyQueuedHits(BtlBakugan *self, int *flinch, int *knockdown)
{
    BtlBakugan *attacker;
    const VtblEntry *entry;
    int hits;
    int total;
    int i;

    if (self->hitQueueCount == 0) {
        return;
    }
    for (i = 0; i < self->hitQueueCount; i++) {
        attacker = BtlBakuganListFind(self->hitQueueAttacker[i]);
        if (attacker == NULL) {
            continue;
        }
        hits = BtlAttackParamsGetHitCount(self->hitQueueType[i]);
        if (self->lockedAttacker != NULL && self->lockedAttacker != attacker) {
            continue;
        }
        total = *flinch;
        *flinch = total + BtlAttackParamsGetFlinchPower(self->hitQueueType[i]) * hits;
        total = *knockdown;
        *knockdown = total + BtlAttackParamsGetKnockdownPower(self->hitQueueType[i]) * hits;
        entry = &((const VtblEntry *)self->base.base.vtable)[10];
        if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
            BtlBakuganAddComboHits(attacker, hits);
        } else {
            entry = &((const VtblEntry *)self->base.base.vtable)[12];
            if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
                BtlBakuganAddComboHits(attacker, hits);
            }
        }
        attacker->linkedUnit = self;
        BtlCombatTakeHit(&self->combat, attacker, 2, self->hitQueueType[i] + 0x23, 0, 0, 6);
    }
    self->hitQueueCount = 0;
}
