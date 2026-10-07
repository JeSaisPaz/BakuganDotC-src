// bdc 0x08882494 BtlAttackType79Update
#include "bdc.h"

/* Per-frame handler of attack type 0x79 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a straight uncancellable shot. Ends after 120 frames (`BtlAttackEnd`). On the first frame
   it scales `vel.xyz` by 40 (w = 0), rotates it about the y axis by `paramF2` radians, copies it
   to its effect's `dir` and normalizes that (w = 0; a zero vector stays zero). Afterwards it runs
   `BtlAttackCheckClash` and sweeps from `pos` along `vel` for hits (hit id 0x9c, kind 3,
   `BtlAttackSweepHit`): a hit spawns impact 0x1b at `g_btlAttackHitPoint`, plays sound
   0x200099 (`BtlAttackPlaySound`) and ends; otherwise `pos.xyz += vel.xyz`. */
void BtlAttackType79Update(BtlAttack *self)
{
    GfxEffect *effect;
    float vx;
    float vz;
    float c;
    float s;
    float lenSq;
    float k;

    if (self->age > 120) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        /* vel.xyz *= 40; vel.w = 0 */
        self->vel[0] = self->vel[0] * 40.0f;
        self->vel[1] = self->vel[1] * 40.0f;
        self->vel[2] = self->vel[2] * 40.0f;
        self->vel[3] = 0.0f;
        /* rotate about y: x' = x cos - z sin, z' = x sin + z cos (angle * 2/pi in quarter turns) */
        c = __builtin_cosf(self->paramF2);
        s = __builtin_sinf(self->paramF2);
        vx = self->vel[0];
        vz = self->vel[2];
        self->vel[0] = vx * c + vz * -s;
        self->vel[2] = vx * s + vz * c;
        effect = (GfxEffect *)self->effect;
        effect->dir[0] = self->vel[0];
        effect->dir[1] = self->vel[1];
        effect->dir[2] = self->vel[2];
        effect->dir[3] = self->vel[3];
        /* effect->dir = normalize(effect->dir), clamped to [-1, 1], w = 0 */
        lenSq = effect->dir[0] * effect->dir[0] + effect->dir[1] * effect->dir[1] +
                effect->dir[2] * effect->dir[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        effect->dir[0] = VfSat1(effect->dir[0] * k);
        effect->dir[1] = VfSat1(effect->dir[1] * k);
        effect->dir[2] = VfSat1(effect->dir[2] * k);
        effect->dir[3] = 0.0f;
        return;
    }
    BtlAttackCheckClash(self);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x9c, 3, 0, 0x31bf337e) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, &g_btlAttackHitPoint.x);
        BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}
