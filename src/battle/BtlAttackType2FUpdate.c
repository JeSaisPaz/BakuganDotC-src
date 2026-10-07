// bdc 0x0887e87c BtlAttackType2FUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x2f (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a homing shot aimed with an offset vector. Once age > 60 it ends (`BtlAttackEnd`); on the first
   frame it does nothing. Afterwards, when its target resolves (`BtlFindBakuganById`), the offset
   is the target-minus-owner vector rotated by a quarter turn about y (angle pi/2 times the bank's
   2/pi: x' = d.(cos, 0, -sin), z' = d.(sin, 0, cos)), flattened (y = 0) and rescaled to length
   paramF2 * 80 (a zero vector takes the bank's 0 as its reciprocal length); its w lane is the
   bank's 0. Without a target the offset is the bank's zero vector C720. Unless
   `BtlAttackResolveClash` (impact 0xd3) consumed it, it homes at speed 100 with height offset 80
   towards target + offset (`BtlAttackSteerToTarget`) and sweeps pos along vel
   (`BtlAttackSweepHit`, hit kind 0x52): on a hit it queues impact 0xd3 at the hit point
   (`BtlAttackSetPendingHit`), otherwise it advances pos.xyz by vel.xyz. */

void BtlAttackType2FUpdate(BtlAttack *self)
{
    float offset[4] BDC_ALIGN16;
    BtlBakugan *target;
    float d[4];
    float c, s;
    float length, lenSq, inv, k;

    if (!((float)self->age <= 60.0f)) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        return;
    }
    target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
    if (target != NULL) {
        /* d = target pos - owner pos (xyz), w from the target */
        d[0] = target->base.pos[0] - self->owner->base.pos[0];
        d[1] = target->base.pos[1] - self->owner->base.pos[1];
        d[2] = target->base.pos[2] - self->owner->base.pos[2];
        d[3] = target->base.pos[3];
        /* vrot of (pi/2 * 2/pi) quarter turns */
        c = __builtin_cosf(VF_PI_2);
        s = __builtin_sinf(VF_PI_2);
        offset[0] = d[0] * c + d[1] * 0.0f + d[2] * -s;
        offset[1] = d[1];
        offset[2] = d[0] * s + d[1] * 0.0f + d[2] * c;
        offset[3] = d[3];
        offset[1] = 0.0f;
        length = self->paramF2 * 80.0f;
        /* offset.xyz = normalize(offset.xyz) * length (0 when |offset| is 0), w = 0 */
        lenSq = offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2];
        inv = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            inv = 0.0f;
        }
        k = inv * length;
        offset[0] = offset[0] * k;
        offset[1] = offset[1] * k;
        offset[2] = offset[2] * k;
        offset[3] = 0.0f;
    } else {
        offset[0] = 0.0f;
        offset[1] = 0.0f;
        offset[2] = 0.0f;
        offset[3] = 0.0f;
    }
    if (BtlAttackResolveClash(self, 0xd3) != 0) {
        return;
    }
    BtlAttackSteerToTarget(100.0f, 80.0f, self, 1, offset);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x52, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xd3, &g_btlAttackHitPoint.x);
        return;
    }
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}
