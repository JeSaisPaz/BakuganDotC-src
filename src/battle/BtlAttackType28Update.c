// bdc 0x088801d8 BtlAttackType28Update
#include "bdc.h"

/* Per-frame handler of attack type 0x28 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   curving homing shot, the same as `BtlAttackType12Update` except the hit kind. Ends (`BtlAttackEnd`) once `age` exceeds 60. On the first frame sets
   `turnRate` 0.03, raises `dir.y` by 0.05, renormalises `dir.xyz` clamped to [-1, 1] (a zero length
   gives a zero vector) with `dir.w` = 0, and rotates it about Y by 1 degree (-1 when `paramF2` is 0).
   On later frames, unless `BtlAttackResolveClash` (impact 0xa8) consumed it, homes at speed 60 /
   height 20 (`BtlAttackSteerToTarget`), sweeps for hits (kind 0x4b, flags 3, `BtlAttackSweepHit`)
   queuing impact 0xa8 at `g_btlAttackHitPoint` on a hit, else advances `pos` by `vel`. Every frame
   not ended by a hit copies `dir` into the effect's `dir`. */
void BtlAttackType28Update(BtlAttack *self)
{
    GfxEffect *effect;
    float lenSq;
    float scale;
    float angle;
    float a;
    float c;
    float s;
    float x;
    float z;

    if (self->age > 0x3c) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->turnRate = 0.0299999993f;
        self->dir[1] = self->dir[1] + 0.0500000007f;
        /* Normalise: scale 0 (bank S713) for a zero length; the C710 store puts S713 = 0 in w. */
        lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1] + self->dir[2] * self->dir[2];
        scale = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            scale = 0.0f;
        }
        self->dir[0] = VfSat1(self->dir[0] * scale);
        self->dir[1] = VfSat1(self->dir[1] * scale);
        self->dir[2] = VfSat1(self->dir[2] * scale);
        self->dir[3] = 0.0f;
        angle = 0.0174532924f;
        if (self->paramF2 == 0.0f) {
            angle = -angle;
        }
        /* Rotate x/z about Y; the angle in quarter turns (times S703 = 2/pi). */
        a = angle * 0.636619747f;
        c = VfCosQuarter(a);
        s = VfSinQuarter(a);
        x = self->dir[0];
        z = self->dir[2];
        self->dir[0] = x * c + self->dir[1] * 0.0f + z * -s;
        self->dir[2] = x * s + self->dir[1] * 0.0f + z * c;
    } else {
        if (BtlAttackResolveClash(self, 0xa8) != 0) {
            return;
        }
        BtlAttackSteerToTarget(60.0f, 20.0f, self, 1, NULL);
        if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x4b, 3, 0, 0x31bf337e) != 0) {
            BtlAttackSetPendingHit(self, 0xa8, &g_btlAttackHitPoint.x);
            return;
        }
        self->pos[0] = self->pos[0] + self->vel[0];
        self->pos[1] = self->pos[1] + self->vel[1];
        self->pos[2] = self->pos[2] + self->vel[2];
    }
    effect = (GfxEffect *)self->effect;
    effect->dir[0] = self->dir[0];
    effect->dir[1] = self->dir[1];
    effect->dir[2] = self->dir[2];
    effect->dir[3] = self->dir[3];
}
