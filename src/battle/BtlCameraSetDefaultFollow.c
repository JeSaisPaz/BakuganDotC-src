// bdc 0x08847460 BtlCameraSetDefaultFollow
#include "bdc.h"

/* Puts the battle camera controller back into its default follow mode 0: clears mode, the
   reset fields and the follow offsets, sets the orbit yaw to the target unit's heading + pi
   (wrapped into (-pi, pi], i.e. behind the unit), sets lookPoint and lookTarget to the unit's
   position raised by the per-kind height kindParams->param[1], and sets followDistance to 450.
   It builds the eye offset: the horizontal vector (cos yaw, 0, sin yaw) of length
   kindParams->param[0], and a quaternion that pitches it by -BtlCameraCalcDefaultPitch() about
   the horizontal axis perpendicular to it. With `snap` set the eye becomes lookPoint plus the
   rotated offset, the look-at becomes lookPoint and eyeSnap = 1 (immediate cut); otherwise
   only eyeSnap = 0 and the camera eases in. */

void BtlCameraSetDefaultFollow(BtlCamera *camera, u8 snap)
{
    GfxModel *unit;
    float yawRot[4];
    float dir[4];
    float orbit[4];
    float conj[4];
    float tmp[4];
    float rotated[4];
    float dist;
    float lenSq;
    float k;
    float pitch;
    float half;
    float s;

    camera->mode = 0;
    camera->turnFrames = 0;
    camera->yawOffset = 0.0f;
    unit = (GfxModel *)camera->target;
    camera->yaw = unit->rot[1] + 3.14159274f;
    if (!(camera->yaw <= 3.14159274f)) {
        camera->yaw = camera->yaw - 6.28318548f;
    } else if (camera->yaw <= -3.14159274f) {
        camera->yaw = camera->yaw + 6.28318548f;
    }
    camera->lockOnFlag = 0;
    unit = (GfxModel *)camera->target;
    camera->lookPoint[0] = unit->pos[0];
    camera->lookPoint[1] = unit->pos[1];
    camera->lookPoint[2] = unit->pos[2];
    camera->lookPoint[3] = unit->pos[3];
    camera->lookPoint[1] = camera->lookPoint[1] + camera->kindParams->param[1];
    camera->lookTarget[0] = camera->lookPoint[0];
    camera->lookTarget[1] = camera->lookPoint[1];
    camera->lookTarget[2] = camera->lookPoint[2];
    camera->lookTarget[3] = camera->lookPoint[3];
    camera->followDistanceOffset = 0.0f;
    camera->followOffsetY = 0.0f;
    camera->followOffset3f4 = 0.0f;

    /* yawRot = (cos yaw, 0, sin yaw, 0) * kindParams->param[0] (vrot [C,0,S,0], w not scaled) */
    dist = camera->kindParams->param[0];
    yawRot[0] = __builtin_cosf(camera->yaw) * dist;
    yawRot[1] = 0.0f * dist;
    yawRot[2] = __builtin_sinf(camera->yaw) * dist;
    yawRot[3] = 0.0f;

    /* dir = (z, y, -x) of yawRot normalised (0 when its length is zero), each lane clamped to
       [-1, 1]; w keeps the bank zero */
    tmp[0] = yawRot[2];
    tmp[1] = yawRot[1];
    tmp[2] = -yawRot[0];
    lenSq = tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2];
    if (lenSq == 0.0f) {
        k = 0.0f;
    } else {
        k = VfRsq(lenSq);
    }
    dir[0] = VfSat1(tmp[0] * k);
    dir[1] = VfSat1(tmp[1] * k);
    dir[2] = VfSat1(tmp[2] * k);
    dir[3] = 0.0f;

    pitch = -BtlCameraCalcDefaultPitch(((GfxModel *)camera->target)->pos[1], camera->basePitch);

    /* orbit = quaternion (dir * sin(pitch / 2), cos(pitch / 2)): the angle in quarter turns is
       pitch * (1 / pi) */
    half = 0.318309873f * pitch;
    s = VfSinQuarter(half);
    orbit[0] = dir[0] * s;
    orbit[1] = dir[1] * s;
    orbit[2] = dir[2] * s;
    orbit[3] = VfCosQuarter(half);

    camera->followDistance = 450.0f;
    if (snap != 0) {
        /* rotated = orbit * (yawRot with w = 0) * conj(orbit): yawRot pitched by the
           quaternion */
        conj[0] = -orbit[0];
        conj[1] = -orbit[1];
        conj[2] = -orbit[2];
        conj[3] = orbit[3];
        yawRot[3] = 0.0f;
        tmp[0] = orbit[0] * yawRot[3] + orbit[1] * yawRot[2] - orbit[2] * yawRot[1] +
                 orbit[3] * yawRot[0];
        tmp[1] = -orbit[0] * yawRot[2] + orbit[1] * yawRot[3] + orbit[2] * yawRot[0] +
                 orbit[3] * yawRot[1];
        tmp[2] = orbit[0] * yawRot[1] - orbit[1] * yawRot[0] + orbit[2] * yawRot[3] +
                 orbit[3] * yawRot[2];
        tmp[3] = -orbit[0] * yawRot[0] - orbit[1] * yawRot[1] - orbit[2] * yawRot[2] +
                 orbit[3] * yawRot[3];
        rotated[0] = tmp[0] * conj[3] + tmp[1] * conj[2] - tmp[2] * conj[1] + tmp[3] * conj[0];
        rotated[1] = -tmp[0] * conj[2] + tmp[1] * conj[3] + tmp[2] * conj[0] + tmp[3] * conj[1];
        rotated[2] = tmp[0] * conj[1] - tmp[1] * conj[0] + tmp[2] * conj[3] + tmp[3] * conj[2];

        /* eye = lookPoint with xyz + rotated (w stays lookPoint.w); look-at = lookPoint */
        camera->base.eye[0] = camera->lookPoint[0] + rotated[0];
        camera->base.eye[1] = camera->lookPoint[1] + rotated[1];
        camera->base.eye[2] = camera->lookPoint[2] + rotated[2];
        camera->base.eye[3] = camera->lookPoint[3];
        camera->base.target[0] = camera->lookPoint[0];
        camera->base.target[1] = camera->lookPoint[1];
        camera->base.target[2] = camera->lookPoint[2];
        camera->base.target[3] = camera->lookPoint[3];
        camera->eyeSnap = 1.0f;
    } else {
        camera->eyeSnap = 0.0f;
    }
}
