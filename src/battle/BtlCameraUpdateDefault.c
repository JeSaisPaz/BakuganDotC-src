// bdc 0x088484b4 BtlCameraUpdateDefault
#include "bdc.h"

/* Camera mode 0 (default follow behind the player unit `target`); does nothing without a target.
   Unless profile flag 0 is set, input action 0x1000000 snaps back to `BtlCameraSetDefaultFollow`.
   The old look point goes to lookTarget and lookPoint eases 35% toward the unit position raised by
   kindParams->param[1]; yaw eases 30% toward the unit heading + pi. turnFrames counts the frames
   the look point moved less than 1 unit (+3 per frame while command bit 0x1000 is set) and is
   cleared otherwise; while it runs yawOffset is pulled toward the current eye heading on an ease
   curve, else it decays by 0.8. The eye is rebuilt around lookPoint at the current eye distance
   eased 40% toward kindParams->param[0], at the current eye heading + yawOffset, tilted by the
   current elevation eased 20% toward `BtlCameraCalcDefaultPitch`; eye and target blend toward
   it / lookPoint by eyeSnap^2 (eyeSnap ramps by 0.1 to 1), and the up vector eases 20% toward
   g_vecUp and is renormalised. Finally `BtlCameraUpdateLockAim` runs when the unit has a target
   (`BtlBakuganGetTarget`) and `BtlCameraApplyCloseUp` always. The VFPU bank constants it
   read (S703 = 2/pi, S713 = 0, S730 = 0) are literals here. */

/* Reduces `a` by trunc(a / pi) whole turns, lifts a negative result by one turn, then returns
   -a below pi and 2*pi - a otherwise: the negated shortest angle of `a`. */
static inline float BtlCameraNegWrapAngle(float a)
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

/* Quaternion product d = s * t, w last (vqmul.q). */
static inline void BtlCameraQuatMul(float *d, const float *s, const float *t)
{
    d[0] = s[0] * t[3] + s[1] * t[2] - s[2] * t[1] + s[3] * t[0];
    d[1] = -s[0] * t[2] + s[1] * t[3] + s[2] * t[0] + s[3] * t[1];
    d[2] = s[0] * t[1] - s[1] * t[0] + s[2] * t[3] + s[3] * t[2];
    d[3] = -s[0] * t[0] - s[1] * t[1] - s[2] * t[2] + s[3] * t[3];
}

void BtlCameraUpdateDefault(BtlCamera *camera)
{
    float tmp[4];
    float rotated[4];
    float pos[4];
    float offset[4];
    float side[4];
    float quat[4];
    float conj[4];
    float half[4];
    BtlBakugan *unit;
    u8 profileFlag;
    float heading;
    float moved;
    float pitch;
    float dist;
    float distance;
    float elevation;
    float tilt;
    float eyeHeading;
    float ramp;
    float cosine;
    float step;
    float delta;
    float snap;
    float len2;
    float scale;
    float angle;
    float sine;
    int i;

    if (camera->target == NULL) {
        return;
    }
    profileFlag = SaveGetProfileFlag0();
    if (profileFlag == 0) {
        unit = (BtlBakugan *)camera->target;
        if ((BtlInputReadActions(unit->input) & 0x1000000) != 0) {
            BtlCameraSetDefaultFollow(camera, 1);
        }
    }

    /* pos = unit position raised by param[1]; lookTarget = lookPoint;
       lookPoint += (pos - lookPoint) * 0.35 (all four lanes) */
    unit = (BtlBakugan *)camera->target;
    for (i = 0; i < 4; i++) {
        pos[i] = unit->base.pos[i];
    }
    pos[1] = pos[1] + camera->kindParams->param[1];
    for (i = 0; i < 4; i++) {
        camera->lookTarget[i] = camera->lookPoint[i];
    }
    for (i = 0; i < 4; i++) {
        camera->lookPoint[i] = camera->lookPoint[i] + (pos[i] - camera->lookPoint[i]) * 0.35f;
    }

    heading = unit->base.rot[1] + 3.1415927f;
    if (!(heading <= 3.1415927f)) {
        heading = heading - 6.2831855f;
    } else if (heading <= -3.1415927f) {
        heading = heading + 6.2831855f;
    }
    camera->yaw = camera->yaw + BtlCameraNegWrapAngle(camera->yaw - heading) * 0.3f;
    if (!(camera->yaw <= 3.1415927f)) {
        camera->yaw = camera->yaw - 6.2831855f;
    } else if (camera->yaw <= -3.1415927f) {
        camera->yaw = camera->yaw + 6.2831855f;
    }

    /* moved = |lookTarget - lookPoint|^2 (xyz) */
    tmp[0] = camera->lookTarget[0] - camera->lookPoint[0];
    tmp[1] = camera->lookTarget[1] - camera->lookPoint[1];
    tmp[2] = camera->lookTarget[2] - camera->lookPoint[2];
    moved = tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2];
    if (moved < 1.0f) {
        if ((((BtlBakugan *)camera->target)->commands & 0x1000) != 0) {
            camera->turnFrames = camera->turnFrames + 2;
        }
        camera->turnFrames = camera->turnFrames + 1;
    } else {
        camera->turnFrames = 0;
    }

    pitch = BtlCameraCalcDefaultPitch(camera->lookPoint[1] - camera->kindParams->param[1],
                                      camera->basePitch);

    /* offset.xyz = eye.xyz - lookPoint.xyz, offset.w = eye.w; dist = |offset.xyz| */
    offset[0] = camera->base.eye[0] - camera->lookPoint[0];
    offset[1] = camera->base.eye[1] - camera->lookPoint[1];
    offset[2] = camera->base.eye[2] - camera->lookPoint[2];
    offset[3] = camera->base.eye[3];
    dist = __builtin_sqrtf(offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2]);
    distance = dist + (camera->kindParams->param[0] - dist) * 0.4f;
    elevation = atan2f(offset[1], __builtin_sqrtf(offset[0] * offset[0] + offset[2] * offset[2]));
    tilt = -elevation + (-pitch - -elevation) * 0.2f;
    eyeHeading = atan2f(offset[2], offset[0]);

    if (camera->turnFrames > 0) {
        ramp = (float)camera->turnFrames * 0.022222223f;
        if (!(ramp <= 1.0f)) {
            ramp = 1.0f;
        }
        delta = BtlCameraNegWrapAngle(eyeHeading - camera->yaw);
        /* vcos.s of (angle * 2/pi) quarter turns = cos(angle) */
        cosine = __builtin_cosf(ramp * ramp * 3.1415927f);
        step = delta * ((1.0f - cosine) * 0.5f) * 0.2f;
        camera->yawOffset = camera->yawOffset +
                            BtlCameraNegWrapAngle(camera->yawOffset - step) * 0.2f;
    } else {
        camera->yawOffset = camera->yawOffset * 0.8f;
    }

    /* offset = (cos a, 0, sin a) * distance, w 0, with a = eyeHeading + yawOffset */
    angle = eyeHeading + camera->yawOffset;
    offset[0] = __builtin_cosf(angle) * distance;
    offset[1] = 0.0f * distance;
    offset[2] = __builtin_sinf(angle) * distance;
    offset[3] = 0.0f;

    /* side = (offset.z, offset.y, -offset.x) normalised (0 for zero length) and saturated to
       [-1, 1], w 0 */
    side[0] = offset[2];
    side[1] = offset[1];
    side[2] = -offset[0];
    len2 = side[0] * side[0] + side[1] * side[1] + side[2] * side[2];
    scale = VfRsq(len2);
    if (len2 == 0.0f) {
        scale = 0.0f;
    }
    side[0] = VfSat1(side[0] * scale);
    side[1] = VfSat1(side[1] * scale);
    side[2] = VfSat1(side[2] * scale);
    side[3] = 0.0f;

    /* quat = (side * sin(tilt / 2), cos(tilt / 2)): tilt / pi quarter turns */
    angle = 0.318309873f * tilt;
    cosine = VfCosQuarter(angle);
    sine = VfSinQuarter(angle);
    quat[0] = side[0] * sine;
    quat[1] = side[1] * sine;
    quat[2] = side[2] * sine;
    quat[3] = cosine;

    snap = camera->eyeSnap + 0.1f;
    if (!(snap <= 1.0f)) {
        snap = 1.0f;
    }
    camera->eyeSnap = snap;
    snap = snap * snap;

    /* rotated = quat * (offset.xyz, 0) * conj(quat) */
    conj[0] = -quat[0];
    conj[1] = -quat[1];
    conj[2] = -quat[2];
    conj[3] = quat[3];
    offset[3] = 0.0f;
    BtlCameraQuatMul(half, quat, offset);
    BtlCameraQuatMul(rotated, half, conj);

    /* tmp = (lookPoint.xyz + rotated.xyz, lookPoint.w); eye += (tmp - eye) * snap;
       target += (lookPoint - target) * snap; up += (g_vecUp - up) * 0.2 */
    tmp[0] = camera->lookPoint[0] + rotated[0];
    tmp[1] = camera->lookPoint[1] + rotated[1];
    tmp[2] = camera->lookPoint[2] + rotated[2];
    tmp[3] = camera->lookPoint[3];
    for (i = 0; i < 4; i++) {
        camera->base.eye[i] = camera->base.eye[i] + (tmp[i] - camera->base.eye[i]) * snap;
    }
    for (i = 0; i < 4; i++) {
        camera->base.target[i] =
            camera->base.target[i] + (camera->lookPoint[i] - camera->base.target[i]) * snap;
    }
    camera->base.up[0] = camera->base.up[0] + (g_vecUp.x - camera->base.up[0]) * 0.2f;
    camera->base.up[1] = camera->base.up[1] + (g_vecUp.y - camera->base.up[1]) * 0.2f;
    camera->base.up[2] = camera->base.up[2] + (g_vecUp.z - camera->base.up[2]) * 0.2f;
    camera->base.up[3] = camera->base.up[3] + (g_vecUp.w - camera->base.up[3]) * 0.2f;

    /* up.xyz normalised (0 for zero length) and saturated to [-1, 1]; up.w = 0 */
    len2 = camera->base.up[0] * camera->base.up[0] + camera->base.up[1] * camera->base.up[1] +
           camera->base.up[2] * camera->base.up[2];
    scale = VfRsq(len2);
    if (len2 == 0.0f) {
        scale = 0.0f;
    }
    camera->base.up[0] = VfSat1(camera->base.up[0] * scale);
    camera->base.up[1] = VfSat1(camera->base.up[1] * scale);
    camera->base.up[2] = VfSat1(camera->base.up[2] * scale);
    camera->base.up[3] = 0.0f;

    if (BtlBakuganGetTarget((BtlBakugan *)camera->target) != NULL) {
        BtlCameraUpdateLockAim(camera, 0);
    }
    BtlCameraApplyCloseUp(camera);
}
