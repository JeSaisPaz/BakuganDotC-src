// bdc 0x08877e88 BtlAttackResolvePendingHit
#include "bdc.h"

/* Resolves an attack's pending hit: when `pendingHit` (`+0xe8`) is set and the impact is not
   suppressed (`BtlLastGuardedHitSuppressesImpact`), plays the hit sound `params.hitSound` (`+0x154`)
   at the attack position `pos` (`BtlAttackPlaySound`) and spawns the impact effect `impactEffect`
   (`+0xec`) at the translation row `mtx[3]` (`+0x50`) with `GfxEffectSpawn` on
   `g_btlAttackEffectMgr`, or on `g_btlUnitEffectMgr` when `useStageEffects` (`+0xf0`) is set; the
   effect's `ownerBakugan` becomes the attack's owner (possibly NULL) and, for a non-NULL owner,
   `ownerId` its object id. In both cases it then clears `pendingHit` and ends the attack
   (`BtlAttackEnd`). Does nothing without a pending hit. */

void BtlAttackResolvePendingHit(BtlAttack *self)
{
    GfxEffect *effect;
    BtlBakugan *owner;

    if (self->pendingHit == 0) {
        return;
    }
    if (BtlLastGuardedHitSuppressesImpact(self) == 0) {
        BtlAttackPlaySound(self, self->params.hitSound, self->pos, 0, 0);
        if (self->useStageEffects == 0) {
            effect = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, self->impactEffect, self->mtx[3]);
        } else {
            effect = (GfxEffect *)GfxEffectSpawn(g_btlUnitEffectMgr, self->impactEffect, self->mtx[3]);
        }
        owner = self->owner;
        effect->ownerBakugan = owner;
        if (owner != NULL) {
            effect->ownerId = owner->base.base.id;
        }
    }
    self->pendingHit = 0;
    BtlAttackEnd(self);
}
