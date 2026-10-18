// bdc 0x088c6e98 GameFieldCameraProbeCollideRadius
#include "bdc.h"

/* Sphere-sweep variant of `GameFieldCameraProbeCollide`. `dir` is the horizontal direction from
   `from` to `to` (y cleared, normalised; (1, 0, 0) when its squared length is below 1e-5). A sweep from
   `to` lowered/raised to the height of `from` to `radius` units behind `from` (`from - radius * dir`)
   with sphere radius `radius` is written to `g_collisionSweptSphereDesc` (start.w = radius^2, dir.w =
   sweep length) and cast through `CollisionRaycast` (layer mask 0x1bf0740, block
   `g_collisionSweptSphereBlock2`). On a hit `outEye` becomes the hit point pushed `radius` units out
   along the hit normal (`g_collisionHitResult`), otherwise a copy of `from`; `outLook` always
   receives a copy of `to`. `probe` is unused. */

void GameFieldCameraProbeCollideRadius(float radius, void *probe, float *outEye, float *outLook, float *from, float *to)
{
    ScePspFVector4 dir;
    ScePspFVector4 start;
    ScePspFVector4 seg;
    float len2;
    float k;
    float negRadius;

    (void)probe;
    dir.x = VfSub(to[0], from[0]);
    dir.y = 0.0f;
    dir.z = VfSub(to[2], from[2]);
    len2 = VfDot3(dir.x, dir.y, dir.z, dir.x, dir.y, dir.z);
    if (len2 < 1e-05f) {
        dir.z = 0.0f;
        dir.x = 1.0f;
    } else {
        len2 = VfDot3(dir.x, dir.y, dir.z, dir.x, dir.y, dir.z);
        k = VfIsZero(len2) ? 0.0f : VfRsq(len2);
        dir.x = VfSat1(VfMul(dir.x, k));
        dir.y = VfSat1(VfMul(dir.y, k));
        dir.z = VfSat1(VfMul(dir.z, k));
    }
    start.x = to[0];
    start.y = from[1];
    start.z = to[2];
    start.w = to[3];
    negRadius = -radius;
    seg.x = VfAdd(VfSub(from[0], start.x), VfMul(dir.x, negRadius));
    seg.y = VfAdd(VfSub(from[1], start.y), VfMul(dir.y, negRadius));
    seg.z = VfAdd(VfSub(from[2], start.z), VfMul(dir.z, negRadius));
    seg.w = from[3];
    g_collisionSweptSphereDesc.start = start;
    g_collisionSweptSphereDesc.dir = seg;
    g_collisionSweptSphereDesc.radius = radius;
    g_collisionSweptSphereDesc.start.w = radius * radius;
    g_collisionSweptSphereDesc.dir.w = VfSqrt(VfDot3(seg.x, seg.y, seg.z, seg.x, seg.y, seg.z));
    if (CollisionRaycast(0x1bf0740, &g_collisionSweptSphereBlock2, 0) != NULL) {
        outEye[0] = VfAdd(g_collisionHitResult.point.x, VfMul(g_collisionHitResult.normal.x, radius));
        outEye[1] = VfAdd(g_collisionHitResult.point.y, VfMul(g_collisionHitResult.normal.y, radius));
        outEye[2] = VfAdd(g_collisionHitResult.point.z, VfMul(g_collisionHitResult.normal.z, radius));
        outEye[3] = g_collisionHitResult.point.w;
    } else {
        outEye[0] = from[0];
        outEye[1] = from[1];
        outEye[2] = from[2];
        outEye[3] = from[3];
    }
    outLook[0] = to[0];
    outLook[1] = to[1];
    outLook[2] = to[2];
    outLook[3] = to[3];
}
