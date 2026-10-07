// bdc 0x088818dc BtlAttackType6AUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x6a (entry 106 of the handler table `0x08a685f0` run by
   `BtlAttackUpdate`): a weaving projectile with attached effect 0x208. Past age 150 it stops
   effect 0x208 and ends. On the first frame it stores the launch delay `param0 = (s32)paramF2`,
   pushes `pos` 80 units sideways (`dir` rotated about Y by -pi/2 with weave step
   `paramF1 = 0.1` when the delay is below 21, else by +pi/2 with -0.1) and 25 up. From frame `param0` on (on that frame setting the effect key to 2 and
   playing `0xd0001f`) it homes with `BtlAttackSteerToTarget(20.0, 100.0)`, tests clashes and
   sweeps for hits; on a hit it spawns effect 0xfb at `g_btlAttackHitPoint` tagged with the
   owner, stops 0x208, plays `0x200098` and ends. Otherwise it advances the weave phase
   `paramF0 += paramF1`, turns `dir` about Y by sin(phase) * 0.03 radians, copies -dir to
   the effect's `dir` and moves `pos` by `vel`. */
void BtlAttackType6AUpdate(BtlAttack *self)
{
    float side[4];
    GfxEffect *effect;
    BtlBakugan *owner;
    float angle;
    float step;
    float turn;
    float c;
    float sn;
    float x;
    float z;

    if (self->age > 150) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x208, self->pos);
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->param0 = (s32)self->paramF2;
        side[0] = self->dir[0];
        side[1] = self->dir[1];
        side[2] = self->dir[2];
        side[3] = self->dir[3];
        if (self->param0 < 21) {
            angle = -1.57079637f;
            step = 0.100000001f;
        } else {
            angle = 1.57079637f;
            step = -0.100000001f;
        }
        /* side.x/z rotated about Y by angle (vrot of angle x 2/pi in quarter turns), y/w kept */
        c = __builtin_cosf(angle);
        sn = __builtin_sinf(angle);
        x = side[0];
        z = side[2];
        side[0] = x * c - z * sn;
        side[2] = x * sn + z * c;
        self->paramF1 = step;
        /* pos.xyz += side.xyz * 80 */
        self->pos[0] = self->pos[0] + side[0] * 80.0f;
        self->pos[1] = self->pos[1] + side[1] * 80.0f;
        self->pos[2] = self->pos[2] + side[2] * 80.0f;
        self->pos[1] = self->pos[1] + 25.0f;
        return;
    }
    if (self->age < self->param0) {
        return;
    }
    if (self->age == self->param0) {
        ((GfxEffect *)self->effect)->key = 2;
        BtlAttackPlaySound(self, 0xd0001f, NULL, 0, 0);
    }
    BtlAttackSteerToTarget(20.0f, 100.0f, self, 1, NULL);
    BtlAttackCheckClash(self);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x8d, 3, 0, 0x31bf337e) != 0) {
        effect = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0xfb, &g_btlAttackHitPoint.x);
        owner = self->owner;
        effect->ownerBakugan = owner;
        if (owner != NULL) {
            effect->ownerId = owner->base.base.id;
        }
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x208, self->pos);
        BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    self->paramF0 = self->paramF0 + self->paramF1;
    /* vsin of phase x 2/pi (quarter turns) = sin(phase) */
    turn = __builtin_sinf(self->paramF0) * 0.0299999993f;
    /* dir.x/z rotated about Y by turn, y/w kept */
    c = __builtin_cosf(turn);
    sn = __builtin_sinf(turn);
    x = self->dir[0];
    z = self->dir[2];
    self->dir[0] = x * c - z * sn;
    self->dir[2] = x * sn + z * c;
    /* effect dir = -dir (xyz), w = dir.w */
    effect = (GfxEffect *)self->effect;
    effect->dir[0] = -self->dir[0];
    effect->dir[1] = -self->dir[1];
    effect->dir[2] = -self->dir[2];
    effect->dir[3] = self->dir[3];
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}
