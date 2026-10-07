// bdc 0x088652b8 BtlBakuganTiltToVelocity
#include "bdc.h"

/* Airborne orientation helper of `BtlBakuganState04Update`/`BtlBakuganState05Update`: sets
   flag 1 in `stateFlags`, keeps the old tilt `{|orient.xz|, orient.y, 0, 0}`, normalises the
   velocity into `g_btlBakuganTiltDir` (xyz saturated to [-1, 1], a zero velocity gives 0; its w
   then becomes the XZ length) and makes `{xz length, y, 0, 0}` the new orientation. It takes
   d = 1 - dot(new tilt, old tilt), turns the orientation about Y by the heading `rot[1]`
   (heading + pi when `motionIdPair` equals `g_btlBakuganTiltFlipMotionIds`), eases it from
   `g_vecUp` by `blend` (`up + (orient - up) * blend`) and renormalises its xyz (w = 0).
   Finally `tiltPush` x and z lose `velocity * d * 5 * step`, and y gains
   `|d| * height * 0.2 * step`, that factor replaced by 15 when it is above 15 (or NaN).
   Returns nothing. */
void BtlBakuganTiltToVelocity(float step, float blend, BtlBakugan *self)
{
    float prevTilt[4];
    float len2;
    float k;
    float dot;
    float angle;
    float c;
    float s;
    float ox;
    float oz;
    float d;
    float rise;

    prevTilt[0] = __builtin_sqrtf(self->orient[0] * self->orient[0] + self->orient[2] * self->orient[2]);
    prevTilt[1] = self->orient[1];
    prevTilt[2] = 0.0f;
    prevTilt[3] = 0.0f;
    self->stateFlags |= 1;

    /* g_btlBakuganTiltDir.xyz = saturate(normalise(velocity.xyz)); .w = 0 (bank S713) */
    len2 = self->base.velocity[0] * self->base.velocity[0] + self->base.velocity[1] * self->base.velocity[1] +
           self->base.velocity[2] * self->base.velocity[2];
    if (len2 == 0.0f) {
        k = 0.0f;
    } else {
        k = VfRsq(len2);
    }
    g_btlBakuganTiltDir.x = VfSat1(self->base.velocity[0] * k);
    g_btlBakuganTiltDir.y = VfSat1(self->base.velocity[1] * k);
    g_btlBakuganTiltDir.z = VfSat1(self->base.velocity[2] * k);
    g_btlBakuganTiltDir.w = 0.0f;
    g_btlBakuganTiltDir.w = __builtin_sqrtf(g_btlBakuganTiltDir.x * g_btlBakuganTiltDir.x +
                                            g_btlBakuganTiltDir.z * g_btlBakuganTiltDir.z);
    self->orient[0] = g_btlBakuganTiltDir.w;
    self->orient[1] = g_btlBakuganTiltDir.y;
    self->orient[2] = 0.0f;
    self->orient[3] = 0.0f;

    /* dot(new tilt, old tilt) */
    dot = self->orient[0] * prevTilt[0] + self->orient[1] * prevTilt[1] + self->orient[2] * prevTilt[2];

    if (self->motionIdPair[0] == g_btlBakuganTiltFlipMotionIds[0] &&
        self->motionIdPair[1] == g_btlBakuganTiltFlipMotionIds[1]) {
        angle = self->base.rot[1] + 3.14159274f;
    } else {
        angle = self->base.rot[1];
    }

    /* orient.xz rotated about Y by the angle; y and w kept */
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    ox = self->orient[0];
    oz = self->orient[2];
    self->orient[0] = ox * c - oz * s;
    self->orient[2] = ox * s + oz * c;

    /* orient = up + (orient - up) * blend */
    self->orient[0] = g_vecUp.x + (self->orient[0] - g_vecUp.x) * blend;
    self->orient[1] = g_vecUp.y + (self->orient[1] - g_vecUp.y) * blend;
    self->orient[2] = g_vecUp.z + (self->orient[2] - g_vecUp.z) * blend;
    self->orient[3] = g_vecUp.w + (self->orient[3] - g_vecUp.w) * blend;

    /* orient.xyz = saturate(normalise(orient.xyz)); .w = 0 (bank S713) */
    len2 = self->orient[0] * self->orient[0] + self->orient[1] * self->orient[1] + self->orient[2] * self->orient[2];
    if (len2 == 0.0f) {
        k = 0.0f;
    } else {
        k = VfRsq(len2);
    }
    self->orient[0] = VfSat1(self->orient[0] * k);
    self->orient[1] = VfSat1(self->orient[1] * k);
    self->orient[2] = VfSat1(self->orient[2] * k);
    self->orient[3] = 0.0f;

    d = 1.0f - dot;
    self->tiltPush[0] = self->tiltPush[0] - self->base.velocity[0] * d * 5.0f * step;
    rise = ABS(d) * self->height * 0.200000003f;
    if (rise <= 15.0f) {
        rise = rise * step;
    } else {
        rise = 15.0f * step;
    }
    self->tiltPush[1] = self->tiltPush[1] + rise;
    self->tiltPush[2] = self->tiltPush[2] - self->base.velocity[2] * d * 5.0f * step;
}
