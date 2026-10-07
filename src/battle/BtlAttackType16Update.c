// bdc 0x0887cf00 BtlAttackType16Update
#include "bdc.h"

/* Per-frame handler of attack type 0x16 (entry 22 of the handler table `0x08a685f0` run by
   `BtlAttackUpdate`). Weaving homing shot: once its age exceeds 150 frames it ends. On frame 0
   it takes the bearing from itself to its owner (atan2f of the z/x differences), subtracts it
   from the owner's heading rot[1], wraps the difference with the binary's (s32)(d * 1/pi) * 2pi
   reduction into [0, 2pi), maps it to -d below pi or 2pi - d otherwise, and stores the sway step
   paramF1 = -0.1 when that is negative, else 0.1. On later frames it homes
   (`BtlAttackSteerToTarget` speed 20, height 100), tests clashes (`BtlAttackCheckClash`) and
   sweeps the hit test along `vel` (hit kind 0x39); on a hit it spawns effect 0xaf at the hit
   point, tags it with the owner and the owner's id, plays sound 0x200098 and ends. Otherwise the
   sway phase paramF0 advances by paramF1, `dir` turns about y by 0.03 x sin(phase) (VFPU sin and
   rotation through the 2/pi bank constant S703, i.e. plain sin/cos in radians), the attached
   effect takes the negated `dir` (w kept) and the position advances by `vel`. */

void BtlAttackType16Update(BtlAttack *self)
{
    float bearing;
    float diff;
    float turn;
    float c;
    float sn;
    float x;
    float z;
    float *edir;
    GfxEffect *hitEffect;
    BtlBakugan *owner;

    if (self->age > 150) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        bearing = atan2f(self->owner->base.pos[2] - self->pos[2],
                         self->owner->base.pos[0] - self->pos[0]);
        diff = self->owner->base.rot[1] - bearing;
        diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
        if (diff < 0.0f) {
            diff = diff + 6.28318548f;
        }
        if (diff < 3.14159274f) {
            diff = -diff;
        } else {
            diff = 6.28318548f - diff;
        }
        if (diff < 0.0f) {
            self->paramF1 = -0.100000001f;
        } else {
            self->paramF1 = 0.100000001f;
        }
        return;
    }
    BtlAttackSteerToTarget(20.0f, 100.0f, self, 1, NULL);
    BtlAttackCheckClash(self);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x39, 3, 0, 0x31bf337e) != 0) {
        hitEffect = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0xaf, &g_btlAttackHitPoint.x);
        owner = self->owner;
        hitEffect->ownerBakugan = owner;
        if (owner != NULL) {
            hitEffect->ownerId = owner->base.base.id;
        }
        BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    self->paramF0 = self->paramF0 + self->paramF1;
    turn = __builtin_sinf(self->paramF0) * 0.0299999993f;
    /* dir rotated about y by turn (vrot [C,0,-S,0] / [S,0,C,0] dotted with dir.xyz); y, w kept */
    c = __builtin_cosf(turn);
    sn = __builtin_sinf(turn);
    x = self->dir[0];
    z = self->dir[2];
    self->dir[0] = x * c - z * sn;
    self->dir[2] = x * sn + z * c;
    /* the attached effect's direction = (-dir.xyz, dir.w); pos.xyz += vel.xyz (w kept) */
    edir = ((GfxEffect *)self->effect)->dir;
    edir[0] = -self->dir[0];
    edir[1] = -self->dir[1];
    edir[2] = -self->dir[2];
    edir[3] = self->dir[3];
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}
