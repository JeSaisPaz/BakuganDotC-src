// bdc 0x0887f3c4 BtlAttackType37Update
#include "bdc.h"

/* Per-frame handler of attack type 0x37 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   an expanding shockwave. Ends the attack (`BtlAttackEnd`) once `age` exceeds 20; on frame 0
   spawns effect 0xe2 at `pos` on `g_btlAttackEffectMgr` unless the owner is airborne
   (`BtlBakuganIsAirborne` with the height check); on frames 1..20 grows `radius` by 5 and sweeps
   it from the attached effect's position to `dir` (`BtlAttackSweepHit`, hit kind 0x5a, arg 3,
   mask `0x31bf337e`). */
void BtlAttackType37Update(BtlAttack *self)
{
    GfxEffect *effect;

    if (self->age > 20) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        if (BtlBakuganIsAirborne(self->owner, 1) == 0) {
            GfxEffectSpawn(g_btlAttackEffectMgr, 0xe2, self->pos);
        }
    } else {
        self->radius = self->radius + 5.0f;
        effect = (GfxEffect *)self->effect;
        BtlAttackSweepHit(self->radius, self, effect->pos, self->dir, 0x5a, 3, 0, 0x31bf337e);
    }
}
