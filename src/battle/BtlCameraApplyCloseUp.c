// bdc 0x08847fe8 BtlCameraApplyCloseUp
#include "bdc.h"

/* Blends the battle camera toward a close-up of its target unit while closeUpFrames runs: each
   frame with frames left, closeUpBlendA rises by 0.0625 and closeUpBlendB by 1/60 (both capped at
   1) and the counter drops; with none left closeUpBlendA falls by 0.1 (floored at 0). Returns
   without touching the pose once closeUpBlendA < 0.0001. Otherwise both factors are eased with
   `(1 - cos(t*pi)) / 2` (easeA, easeB). The look-at point starts at the unit's position raised by
   0.4 * its stat-table height; the close-up eye sits on a horizontal circle of radius 250 (450 for
   unit kind 10) around it at the unit's heading + (25 - 55*easeB) degrees, raised by
   200*easeB - 80 (kind 10: that * 1.8 + 40). The look-at is pulled toward the "Bip01" bone plus a
   heading offset of the same radius by easeB * closeUpParam and raised by
   closeUpDistance * closeUpParam. Finally the camera eye, look-at and up vector are each blended
   by easeA toward that eye, that look-at and g_vecUp, and the up vector is renormalised (up.w
   cleared). */

void BtlCameraApplyCloseUp(BtlCamera *camera)
{
    ScePspFVector4 node;
    float lookAt[4];
    float eye[4];
    float boneOffset[4];
    BtlBakugan *unit;
    float blend;
    float angle;
    float lenSq;
    float scale;
    s32 i;
    float easeA;
    float easeB;
    float radius;
    float height;
    float angleDeg;
    float pull;

    if (camera->closeUpFrames > 0) {
        blend = camera->closeUpBlendA + 0.0625f;
        if (!(blend <= 1.0f)) {
            blend = 1.0f;
        }
        camera->closeUpBlendA = blend;
        blend = camera->closeUpBlendB + 0.016666668f;
        if (!(blend <= 1.0f)) {
            blend = 1.0f;
        }
        camera->closeUpBlendB = blend;
        camera->closeUpFrames = camera->closeUpFrames - 1;
    } else {
        blend = camera->closeUpBlendA - 0.1f;
        if (blend < 0.0f) {
            blend = 0.0f;
        }
        camera->closeUpBlendA = blend;
    }
    if (camera->closeUpBlendA < 0.0001f) {
        return;
    }

    /* vcos of angle * 2/pi (bank S703) in quarter turns: the cosine of the angle */
    easeA = (1.0f - __builtin_cosf(camera->closeUpBlendA * 3.14159274f)) * 0.5f;
    easeB = (1.0f - __builtin_cosf(camera->closeUpBlendB * 3.14159274f)) * 0.5f;

    radius = 250.0f;
    height = easeB * 200.0f + -80.0f;
    angleDeg = easeB * -55.0f + 25.0f;
    if (((BtlBakugan *)camera->target)->base.base.unk08 == 10) {
        height = height * 1.8f;
        radius = radius * 1.8f;
        height = height + 40.0f;
    }

    unit = (BtlBakugan *)camera->target;
    for (i = 0; i < 4; i++) {
        lookAt[i] = unit->base.pos[i];
    }
    unit = (BtlBakugan *)camera->target;
    lookAt[1] = lookAt[1] + unit->combat.stats->height * 0.4f;

    /* eye = lookAt.xyz + (cos a, 0, sin a) * radius, a = heading + angleDeg in radians; eye.w = 0 */
    angle = unit->base.rot[1] + angleDeg * 0.017453292f;
    eye[0] = __builtin_cosf(angle) * radius;
    eye[1] = 0.0f;
    eye[2] = __builtin_sinf(angle) * radius;
    eye[3] = 0.0f;
    for (i = 0; i < 3; i++) {
        eye[i] = eye[i] + lookAt[i];
    }
    eye[1] = eye[1] + height;

    /* boneOffset = (cos h, 0, sin h, 0) * radius for the unit heading h */
    unit = (BtlBakugan *)camera->target;
    angle = unit->base.rot[1];
    boneOffset[0] = __builtin_cosf(angle) * radius;
    boneOffset[1] = 0.0f;
    boneOffset[2] = __builtin_sinf(angle) * radius;
    boneOffset[3] = 0.0f;
    GfxModelGetNodeWorldPos((GfxModel *)camera->target, &node, "Bip01");

    /* boneOffset.xyz += node.xyz; lookAt += (boneOffset - lookAt) * pull (all four lanes) */
    boneOffset[0] = boneOffset[0] + node.x;
    boneOffset[1] = boneOffset[1] + node.y;
    boneOffset[2] = boneOffset[2] + node.z;
    pull = easeB * camera->closeUpParam;
    for (i = 0; i < 4; i++) {
        lookAt[i] = lookAt[i] + (boneOffset[i] - lookAt[i]) * pull;
    }
    lookAt[1] = lookAt[1] + camera->closeUpDistance * camera->closeUpParam;

    /* base.eye/target/up += (eye/lookAt/g_vecUp - them) * easeA */
    for (i = 0; i < 4; i++) {
        camera->base.eye[i] = camera->base.eye[i] + (eye[i] - camera->base.eye[i]) * easeA;
    }
    for (i = 0; i < 4; i++) {
        camera->base.target[i] = camera->base.target[i] + (lookAt[i] - camera->base.target[i]) * easeA;
    }
    camera->base.up[0] = camera->base.up[0] + (g_vecUp.x - camera->base.up[0]) * easeA;
    camera->base.up[1] = camera->base.up[1] + (g_vecUp.y - camera->base.up[1]) * easeA;
    camera->base.up[2] = camera->base.up[2] + (g_vecUp.z - camera->base.up[2]) * easeA;
    camera->base.up[3] = camera->base.up[3] + (g_vecUp.w - camera->base.up[3]) * easeA;

    /* up.xyz normalised (scale 0 when its length is zero) and saturated to [-1, 1]; up.w = 0 (bank S713) */
    lenSq = camera->base.up[0] * camera->base.up[0] + camera->base.up[1] * camera->base.up[1]
          + camera->base.up[2] * camera->base.up[2];
    scale = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    camera->base.up[0] = VfSat1(camera->base.up[0] * scale);
    camera->base.up[1] = VfSat1(camera->base.up[1] * scale);
    camera->base.up[2] = VfSat1(camera->base.up[2] * scale);
    camera->base.up[3] = 0.0f;
}
