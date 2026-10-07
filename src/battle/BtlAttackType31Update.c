// bdc 0x0887ea44 BtlAttackType31Update
#include "bdc.h"

/* Per-frame handler of attack type 0x31 (entry 49 of the attack handler table run by
   `BtlAttackUpdate`). After 120 frames of age it stops its attached effect 0x200, spawns effect
   0xbd at its position (owned by the attack's owner Bakugan) and ends. On frame 0 it clears the
   turn rate, turns `dir` about y by 120 degrees (side variant `(s32)paramF2 == 1`) or 240 degrees
   (variant 2), and sets the homing speed `mtx[0][0]` to 30 and the rise `paramF2` to 8. On later
   frames it homes (`BtlAttackSteerToTarget` with that speed, height 100); from frame 5 on it
   sweeps the hit test along `vel` (hit kind 0x41) and on a hit stops 0x200, spawns 0xbd, plays
   sound 0x200098 and ends. Otherwise the speed drops by 0.3 (to 0 when the result's bits are not
   positive, i.e. <= 0 or negative zero), the position advances by `vel` plus the rise in y, and
   the rise decays by 0.8. The rotation is the vrot.q of angle * S703 (bank 2/pi), i.e.
   cosf/sinf of the angle in radians. */

void BtlAttackType31Update(BtlAttack *self)
{
    float *pos;
    GfxEffect *effect;
    BtlBakugan *owner;
    float angle;
    float c;
    float s;
    float x;
    float y;
    float z;
    s32 variant;
    union { float f; s32 i; } speed;

    if (self->age > 120) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x200, self->pos);
        effect = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0xbd, self->pos);
        owner = self->owner;
        effect->ownerBakugan = owner;
        if (owner != NULL) {
            effect->ownerId = owner->base.base.id;
        }
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        variant = (s32)self->paramF2;
        self->turnRate = 0.0f;
        if (variant > 0 && variant < 3) {
            angle = (variant < 2) ? 2.09439516f : 4.18879032f;
            /* dir x/z rotated by angle about y (vrot.q of angle * 2/pi): C000 = (c, 0, -s, 0),
               C010 = (s, 0, c, 0); x' = dot3(dir, C000), z' = dot3(dir, C010); y, w kept */
            c = __builtin_cosf(angle);
            s = __builtin_sinf(angle);
            x = self->dir[0];
            y = self->dir[1];
            z = self->dir[2];
            self->dir[0] = x * c + y * 0.0f + z * -s;
            self->dir[2] = x * s + y * 0.0f + z * c;
        }
        self->mtx[0][0] = 30.0f;
        self->paramF2 = 8.0f;
        return;
    }
    BtlAttackSteerToTarget(self->mtx[0][0], 100.0f, self, 1, NULL);
    pos = self->pos;
    if (self->age >= 5
        && BtlAttackSweepHit(self->radius, self, pos, self->vel, 0x41, 3, 0, 0x31bf337e) != 0) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x200, pos);
        effect = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0xbd, pos);
        owner = self->owner;
        effect->ownerBakugan = owner;
        if (owner != NULL) {
            effect->ownerId = owner->base.base.id;
        }
        BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    speed.f = self->mtx[0][0] - 0.300000012f;
    if (speed.i <= 0) {
        speed.f = 0.0f;
    }
    self->mtx[0][0] = speed.f;
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
    self->pos[1] = self->pos[1] + self->paramF2;
    self->paramF2 = self->paramF2 * 0.800000012f;
}
