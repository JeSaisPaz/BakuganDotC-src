// bdc 0x088476a8 BtlCameraUpdateLockAim
#include "bdc.h"

/* Puts the camera into lock-on mode (mode 1) framing the player unit `target` and its foe
   (`BtlBakuganGetTarget`, not NULL-checked). `near` = 1.2 - horizontal distance / 1300,
   capped to 1. kindParam3 = kindParams->param[3] minus the foe height step
   ((foeY - unitY) * 0.1 clamped to -100..param[3], halved when negative); the follow offsets
   and lockOnFlag are cleared. lookPoint (copied to lookTarget) is the unit position and
   focusPoint (copied to prevFocusPoint) the unit-to-foe midpoint, both raised by kindParam3.
   The yaw from eye to lookPoint (`atan2f`) is turned just enough to keep the focus inside an
   80-degree cone narrowed by (1 - near) * 0.7, and stored. lockOnElevation is set to
   `BtlCameraCalcLockOnPitch` (weight 1). With `snap` the eye is placed at lookPoint plus a
   param[2]-long offset along the yaw, tilted by -lockOnElevation, the target at focusPoint and
   eyeSnap = 1; otherwise eyeSnap = 0. lockOnCatchUp, lockOnMidBlend (1) and lockOnHoldFrames
   (0) are reset. */

/* Reduces `a` by trunc(a / pi) whole turns, lifts a negative result by one turn, then returns
   -a below pi and 2*pi - a otherwise: the negated shortest angle of `a`. */
static inline float BtlLockAimNegWrapAngle(float a)
{
    a = a - (float)(int)(a * 0.31830987f) * 6.2831855f;
    if (a < 0.0f) {
        a = a + 6.2831855f;
    }
    if (a < 3.1415927f) {
        return -a;
    }
    return 6.2831855f - a;
}

/* Quaternion product s * t, w last (vqmul.q). */
static inline void BtlLockAimQuatMul(float *d, const float *s, const float *t)
{
    d[0] = s[0] * t[3] + s[1] * t[2] - s[2] * t[1] + s[3] * t[0];
    d[1] = -s[0] * t[2] + s[1] * t[3] + s[2] * t[0] + s[3] * t[1];
    d[2] = s[0] * t[1] - s[1] * t[0] + s[2] * t[3] + s[3] * t[2];
    d[3] = -s[0] * t[0] - s[1] * t[1] - s[2] * t[2] + s[3] * t[3];
}

void BtlCameraUpdateLockAim(BtlCamera *camera, char snap)
{
    float quat[4];
    float conj[4];
    float offset[4];
    float half[4];
    float rotated[4];
    float side[4];
    float k;
    float lenSq;
    float rs;
    float a;
    float s;
    float c;
    BtlBakugan *foe;
    BtlBakugan *unit;
    float dx;
    float dz;
    float dist;
    float scaled;
    float near;
    float rise;
    float eyeAngle;
    float diff;
    float outer;
    int i;

    foe = (BtlBakugan *)BtlBakuganGetTarget((BtlBakugan *)camera->target);

    /* dist = horizontal distance unit -> foe (y lane zeroed) */
    unit = (BtlBakugan *)camera->target;
    dx = foe->base.pos[0] - unit->base.pos[0];
    dz = foe->base.pos[2] - unit->base.pos[2];
    dist = __builtin_sqrtf(dx * dx + dz * dz);
    scaled = dist * 0.00076923077f;
    if (!(scaled <= 1.2f)) {
        scaled = 1.2f;
    }
    near = 1.2f - scaled;
    if (!(near <= 1.0f)) {
        near = 1.0f;
    }

    camera->mode = 1;
    rise = (foe->base.pos[1] - ((BtlBakugan *)camera->target)->base.pos[1]) * 0.1f;
    if (rise < -100.0f) {
        rise = -100.0f;
    } else if (camera->kindParams->param[3] < rise) {
        rise = camera->kindParams->param[3];
    }
    if (rise < 0.0f) {
        rise = rise * 0.5f;
    }
    camera->kindParam3 = camera->kindParams->param[3] - rise;
    camera->followDistanceOffset = 0.0f;
    camera->followOffsetY = 0.0f;
    camera->followOffset3f4 = 0.0f;
    camera->lockOnFlag = 0;

    /* lookPoint = unit position raised by kindParam3; lookTarget = lookPoint */
    unit = (BtlBakugan *)camera->target;
    for (i = 0; i < 4; i++) {
        camera->lookPoint[i] = unit->base.pos[i];
    }
    camera->lookPoint[1] = camera->lookPoint[1] + camera->kindParam3;
    for (i = 0; i < 4; i++) {
        camera->lookTarget[i] = camera->lookPoint[i];
    }

    /* focusPoint = unit position + (foe position - unit position) * 0.5, raised by kindParam3;
       prevFocusPoint = focusPoint */
    unit = (BtlBakugan *)camera->target;
    for (i = 0; i < 4; i++) {
        camera->focusPoint[i] = unit->base.pos[i];
    }
    for (i = 0; i < 4; i++) {
        half[i] = camera->focusPoint[i];
    }
    for (i = 0; i < 4; i++) {
        camera->focusPoint[i] = half[i] + (foe->base.pos[i] - half[i]) * 0.5f;
    }
    camera->focusPoint[1] = camera->focusPoint[1] + camera->kindParam3;
    for (i = 0; i < 4; i++) {
        camera->prevFocusPoint[i] = camera->focusPoint[i];
    }

    /* yaw of eye -> lookPoint against the yaw of lookPoint -> focusPoint */
    eyeAngle = atan2f(camera->lookPoint[2] - camera->base.eye[2],
                      camera->lookPoint[0] - camera->base.eye[0]);
    diff = BtlLockAimNegWrapAngle(
        eyeAngle - atan2f(camera->focusPoint[2] - camera->lookPoint[2],
                          camera->focusPoint[0] - camera->lookPoint[0]));

    /* turn only past the 80-degree cone, narrowed as the foe gets farther */
    outer = 1.3962634f - (1.0f - near) * 1.3962634f * 0.7f;
    if (diff < 0.0f) {
        if (diff < -outer) {
            eyeAngle = (diff + outer) + eyeAngle;
        }
    } else if (!(diff <= outer)) {
        eyeAngle = eyeAngle + (diff - outer);
    }
    camera->yaw = eyeAngle;

    /* offset = (cos yaw, 0, sin yaw, 0) * -param[2] (vrot of yaw * 2/pi, vscl.t) */
    k = -camera->kindParams->param[2];
    offset[0] = __builtin_cosf(eyeAngle) * k;
    offset[1] = 0.0f * k;
    offset[2] = __builtin_sinf(eyeAngle) * k;
    offset[3] = 0.0f;

    camera->lockOnElevation = BtlCameraCalcLockOnPitch(
        camera->focusPoint[1] - camera->kindParam3, ((BtlBakugan *)camera->target)->base.pos[1],
        camera->lockOnPitch, 1.0f);

    /* side = normalised (offset.z, offset.y, -offset.x) saturated to [-1, 1] (0 for a zero
       vector), w = 0; quat = (side * sin(t / 2), cos(t / 2)) for t = -lockOnElevation */
    side[0] = offset[2];
    side[1] = offset[1];
    side[2] = -offset[0];
    lenSq = side[0] * side[0] + side[1] * side[1] + side[2] * side[2];
    rs = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        rs = 0.0f;
    }
    side[0] = VfSat1(side[0] * rs);
    side[1] = VfSat1(side[1] * rs);
    side[2] = VfSat1(side[2] * rs);
    side[3] = 0.0f;
    a = 0.31830987f * -camera->lockOnElevation;
    c = VfCosQuarter(a);
    s = VfSinQuarter(a);
    quat[0] = side[0] * s;
    quat[1] = side[1] * s;
    quat[2] = side[2] * s;
    quat[3] = c;

    camera->lockOnCatchUp = 0.0f;
    if (snap != 0) {
        /* rotated = quat * (offset.xyz, 0) * conj(quat); eye = lookPoint + rotated (w kept) */
        conj[0] = -quat[0];
        conj[1] = -quat[1];
        conj[2] = -quat[2];
        conj[3] = quat[3];
        offset[3] = 0.0f;
        BtlLockAimQuatMul(half, quat, offset);
        BtlLockAimQuatMul(rotated, half, conj);
        camera->base.eye[0] = camera->lookPoint[0] + rotated[0];
        camera->base.eye[1] = camera->lookPoint[1] + rotated[1];
        camera->base.eye[2] = camera->lookPoint[2] + rotated[2];
        camera->base.eye[3] = camera->lookPoint[3];
        for (i = 0; i < 4; i++) {
            camera->base.target[i] = camera->focusPoint[i];
        }
        camera->eyeSnap = 1.0f;
    } else {
        camera->eyeSnap = 0.0f;
    }
    camera->lockOnMidBlend = 1.0f;
    camera->lockOnHoldFrames = 0;
}
