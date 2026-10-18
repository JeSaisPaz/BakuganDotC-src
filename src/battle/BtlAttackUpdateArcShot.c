// bdc 0x0887d910 BtlAttackUpdateArcShot
#include "bdc.h"

/* Shared update of a sideways-launched homing shot (attack types 0x1d and 0x65). After 60 frames
   of age it ends. On frame 0 it lowers the position by 20 and moves it 10 units along the
   horizontal velocity direction turned by +90 degrees about y (paramF2 == 0) or -90 degrees
   (otherwise), turns `dir` by 0.03 of that angle and sets the turn rate to 0.02. On later frames,
   unless `BtlAttackResolveClash` (impact effect 0xbb) consumed it, it homes
   (`BtlAttackSteerToTarget` speed 35, height 80), sweeps the hit test along `vel` with hit kind
   `hitKind` and on a hit queues impact effect 0xbb at the hit point
   (`BtlAttackSetPendingHit`); otherwise the position advances by `vel`. */

void BtlAttackUpdateArcShot(BtlAttack *self, s32 hitKind)
{
    float offset[4];
    float lenSq;
    float scale;
    float angle;
    float c;
    float s;
    float x;
    float y;
    float z;

    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->pos[1] = self->pos[1] + -20.0f;
        /* offset = vel with y cleared, then offset.xyz = normalize(offset.xyz) * 10 (a zero length
           scales by the bank's S713 = 0); the result passes through C710, so w = S713 = 0. */
        offset[0] = self->vel[0];
        offset[1] = 0.0f;
        offset[2] = self->vel[2];
        lenSq = VfDot3(offset[0], offset[1], offset[2], offset[0], offset[1], offset[2]);
        scale = VfIsZero(lenSq) ? 0.0f : VfRsq(lenSq);
        scale = VfMul(scale, 10.0f);
        offset[0] = VfMul(offset[0], scale);
        offset[1] = VfMul(offset[1], scale);
        offset[2] = VfMul(offset[2], scale);
        offset[3] = 0.0f;
        angle = 1.57079637f;
        if (!(self->paramF2 == 0.0f)) {
            angle = -1.57079637f;
        }
        /* vrot with angle * S703 (2/pi) in quarter turns: cos/sin of the angle in radians.
           offset x/z rotated about y: x' = c*x - s*z, z' = s*x + c*z (y, w kept). */
        c = VfCosQuarter(VfMul(angle, 0.636619747f));
        s = VfSinQuarter(VfMul(angle, 0.636619747f));
        x = offset[0];
        y = offset[1];
        z = offset[2];
        offset[0] = VfDot3(x, y, z, c, 0.0f, -s);
        offset[2] = VfDot3(x, y, z, s, 0.0f, c);
        /* dir rotated the same way by angle * 0.03 */
        angle = angle * 0.0299999993f;
        c = VfCosQuarter(VfMul(angle, 0.636619747f));
        s = VfSinQuarter(VfMul(angle, 0.636619747f));
        x = self->dir[0];
        y = self->dir[1];
        z = self->dir[2];
        self->dir[0] = VfDot3(x, y, z, c, 0.0f, -s);
        self->dir[2] = VfDot3(x, y, z, s, 0.0f, c);
        self->turnRate = 0.0199999996f;
        /* pos.xyz += offset.xyz (w kept) */
        self->pos[0] = VfAdd(self->pos[0], offset[0]);
        self->pos[1] = VfAdd(self->pos[1], offset[1]);
        self->pos[2] = VfAdd(self->pos[2], offset[2]);
        return;
    }
    if (BtlAttackResolveClash(self, 0xbb) != 0) {
        return;
    }
    BtlAttackSteerToTarget(35.0f, 80.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitKind, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xbb, &g_btlAttackHitQuery.contact.x);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = VfAdd(self->pos[0], self->vel[0]);
    self->pos[1] = VfAdd(self->pos[1], self->vel[1]);
    self->pos[2] = VfAdd(self->pos[2], self->vel[2]);
}
