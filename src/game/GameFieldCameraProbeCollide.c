// bdc 0x088c6cfc GameFieldCameraProbeCollide
#include "bdc.h"

/* Camera wall test of the field camera helpers. `dir` is the horizontal direction from `eye` to
   `look` (y cleared, normalised with each lane clamped to [-1, 1]; (1, 0, 0) when its squared length
   is below 1e-5). A segment from `look` lowered/raised to the eye height to 3 units behind `eye`
   (`eye - 3 * dir`) is written to `g_collisionSegmentDesc` and cast through `CollisionRaycast`
   (layer mask 0x1bf0740, block `g_collisionSegmentBlock2`). On a hit `outEye` becomes the hit point
   pushed 3 units out along the hit normal (`g_collisionHitResult`), otherwise a copy of `eye`;
   `outLook` always receives a copy of `look`. `probe` is unused. The w lane of the segment start is
   `look[3]`, of the segment direction `eye[3]`. */

void GameFieldCameraProbeCollide(void *probe, float *outEye, float *outLook, float *eye, float *look)
{
    float dir[3];
    float start[4];
    float seg[4];
    float len2;
    float k;

    (void)probe;
    dir[0] = look[0] - eye[0];
    dir[1] = 0.0f;
    dir[2] = look[2] - eye[2];
    len2 = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    if (len2 < 1e-05f) {
        dir[2] = 0.0f;
        dir[0] = 1.0f;
    } else {
        k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
        dir[0] = VfSat1(dir[0] * k);
        dir[1] = VfSat1(dir[1] * k);
        dir[2] = VfSat1(dir[2] * k);
    }
    start[0] = look[0];
    start[1] = eye[1];
    start[2] = look[2];
    start[3] = look[3];
    seg[0] = eye[0] - start[0];
    seg[1] = eye[1] - start[1];
    seg[2] = eye[2] - start[2];
    seg[3] = eye[3];
    seg[0] = seg[0] + dir[0] * -3.0f;
    seg[1] = seg[1] + dir[1] * -3.0f;
    seg[2] = seg[2] + dir[2] * -3.0f;
    g_collisionSegmentDesc.start[0] = start[0];
    g_collisionSegmentDesc.start[1] = start[1];
    g_collisionSegmentDesc.start[2] = start[2];
    g_collisionSegmentDesc.start[3] = start[3];
    g_collisionSegmentDesc.dir[0] = seg[0];
    g_collisionSegmentDesc.dir[1] = seg[1];
    g_collisionSegmentDesc.dir[2] = seg[2];
    g_collisionSegmentDesc.dir[3] = seg[3];
    if (CollisionRaycast(0x1bf0740, &g_collisionSegmentBlock2, 0) != NULL) {
        outEye[0] = g_collisionHitResult.point.x;
        outEye[1] = g_collisionHitResult.point.y;
        outEye[2] = g_collisionHitResult.point.z;
        outEye[3] = g_collisionHitResult.point.w;
        outEye[0] = outEye[0] + g_collisionHitResult.normal.x * 3.0f;
        outEye[1] = outEye[1] + g_collisionHitResult.normal.y * 3.0f;
        outEye[2] = outEye[2] + g_collisionHitResult.normal.z * 3.0f;
    } else {
        outEye[0] = eye[0];
        outEye[1] = eye[1];
        outEye[2] = eye[2];
        outEye[3] = eye[3];
    }
    outLook[0] = look[0];
    outLook[1] = look[1];
    outLook[2] = look[2];
    outLook[3] = look[3];
}
