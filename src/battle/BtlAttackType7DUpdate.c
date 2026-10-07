// bdc 0x08882af8 BtlAttackType7DUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x7d (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   falling strike. After frame 60 it ends (`BtlAttackEnd`). On frame 0 it sets the velocity to
   (0, -100, 0, 0), points its effect along `g_vecDown` and plays sound 0x100001f. Later frames
   test clashes (`BtlAttackCheckClash`) and sweep for hits (hit kind 0xa0, arg 3); on a hit they
   spawn impact 0x1b at `g_btlAttackHitPoint` when `g_btlAttackHitCollider` is set (else 0x103),
   play 0x200099 and end; otherwise move the position by the velocity (xyz, w kept). */

void BtlAttackType7DUpdate(BtlAttack *self)
{
    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->vel[0] = 0.0f;
        self->vel[1] = -100.0f;
        self->vel[2] = 0.0f;
        self->vel[3] = 0.0f;
        ((GfxEffect *)self->effect)->dir[0] = g_vecDown.x;
        ((GfxEffect *)self->effect)->dir[1] = g_vecDown.y;
        ((GfxEffect *)self->effect)->dir[2] = g_vecDown.z;
        ((GfxEffect *)self->effect)->dir[3] = g_vecDown.w;
        BtlAttackPlaySound(self, 0x100001f, NULL, 0, 0);
        return;
    }
    BtlAttackCheckClash(self);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0xa0, 3, 0, 0x31bf337e) != 0) {
        if (g_btlAttackHitCollider != NULL) {
            GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, &g_btlAttackHitPoint.x);
        } else {
            GfxEffectSpawn(g_btlAttackEffectMgr, 0x103, &g_btlAttackHitPoint.x);
        }
        BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}
