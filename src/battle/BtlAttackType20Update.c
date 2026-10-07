// bdc 0x0887de64 BtlAttackType20Update
#include "bdc.h"

/* Per-frame handler of attack type 0x20 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   on the first frame (`age` 0) sets `turnRate` to 0.08, then runs `BtlAttackUpdateGuided` with
   turn 0.1, speed 0, hit kind 0x43, hit effect 0xbf and cancel effect 0xbf. When that returns
   non-zero (the attack ended) it plays sound 0x200098 at the attack position
   (`BtlAttackPlaySound`) and stops every effect of `g_btlAttackEffectMgr` attached to the
   attack's `pos` (`GfxEffectStopAttached`, any id); otherwise, when the attack has an attached
   `effect`, it sets the effect's `dir` to the attack's velocity with xyz negated (w copied). */
void BtlAttackType20Update(BtlAttack *self)
{
    float *dir;

    if (self->age == 0) {
        self->turnRate = 0.0799999982f;
    }
    if (BtlAttackUpdateGuided(0.100000001f, 0.0f, self, 0x43, 0xbf, 0xbf) != 0) {
        BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
        GfxEffectStopAttached(g_btlAttackEffectMgr, -1, self->pos);
        return;
    }
    if (self->effect == NULL) {
        return;
    }
    dir = ((GfxEffect *)self->effect)->dir;
    dir[0] = -self->vel[0];
    dir[1] = -self->vel[1];
    dir[2] = -self->vel[2];
    dir[3] = self->vel[3];
}
