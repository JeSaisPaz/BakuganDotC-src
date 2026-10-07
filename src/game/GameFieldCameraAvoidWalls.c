// bdc 0x088bae7c GameFieldCameraAvoidWalls
#include "bdc.h"

/* Wall avoidance of the field camera: sweeps a 3.3-radius sphere (`g_collisionSweptSphereDesc`,
   raised 8 units above the look-at point) from the look-at point towards the eye with
   `CollisionRaycast` (mask `0x37bf0700`). A hit closer than `distance` snaps the eye onto the hit
   point, shrinks `distance` and arms `wallAvoidTimer` (30 frames). Otherwise, once the timer has run
   out (or the follow goal is still moving), a second sweep at the default distance
   (`g_gameFieldCameraDefaultDistance`) eases `distance` back out along a cosine ramp. Finally the
   near plane is set from the eye/look-at distance (0.3 × distance below 7.8, else 3.9); after a snap
   that distance is the hit distance. Bank constants: S713 = 0.0f (zero-length fallback of the
   normalisations, `w` lane of the `vscl.t` results, lower clamp), S733 = 1.0f (upper clamp),
   S703 = 2/π (radians → quarter turns for `vcos.s`). */

void GameFieldCameraAvoidWalls(GameFieldCamera *cam)
{
    ScePspFVector4 eyeOfs;
    ScePspFVector4 tmp;
    ScePspFVector4 hit;
    ScePspFVector4 newEye;
    ScePspFVector4 hit2;
    CollisionSweptSphereDesc *desc;
    float goalDist2;
    float dist;
    float len;
    float t;
    float d2;
    float r;
    float blend;
    float ramp;
    float c;
    float step;
    float lim;
    u8 goalMoving;
    int moved;
    int timer;

    /* goalDist2 = |followGoal - followLookAt|² (xyz) */
    {
        float dx = cam->followGoal.x - cam->followLookAt.x;
        float dy = cam->followGoal.y - cam->followLookAt.y;
        float dz = cam->followGoal.z - cam->followLookAt.z;
        goalDist2 = dx * dx + dy * dy + dz * dz;
    }
    goalMoving = 0;
    if (!(goalDist2 <= 0.1f)) {
        goalMoving = 1;
    }

    desc = &g_collisionSweptSphereDesc;
    /* eyeOfs.xyz = eye - target (w = eye.w) */
    eyeOfs.x = cam->base.eye[0] - cam->base.target[0];
    eyeOfs.y = cam->base.eye[1] - cam->base.target[1];
    eyeOfs.z = cam->base.eye[2] - cam->base.target[2];
    eyeOfs.w = cam->base.eye[3];
    desc->radius = 3.3f;
    desc->start.x = cam->base.target[0];
    desc->start.y = cam->base.target[1];
    desc->start.z = cam->base.target[2];
    desc->start.w = cam->base.target[3];
    desc->start.y = desc->start.y + 8.0f;
    desc->dir = eyeOfs;
    moved = 0;
    dist = 1.0f;

    if (CollisionRaycast(0x37bf0700, desc->shapeBlock, 1) != NULL) {
        /* hit = dir * bestT; dir = normalize(dir) * 4; hit += dir; dist = |hit| */
        t = g_collisionHitInfo.bestT;
        hit.x = desc->dir.x * t;
        hit.y = desc->dir.y * t;
        hit.z = desc->dir.z * t;
        hit.w = 0.0f;
        d2 = desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z;
        r = (d2 == 0.0f) ? 0.0f : VfRsq(d2);
        r = r * 4.0f;
        desc->dir.x = desc->dir.x * r;
        desc->dir.y = desc->dir.y * r;
        desc->dir.z = desc->dir.z * r;
        desc->dir.w = 0.0f;
        hit.x = hit.x + desc->dir.x;
        hit.y = hit.y + desc->dir.y;
        hit.z = hit.z + desc->dir.z;
        dist = __builtin_sqrtf(hit.x * hit.x + hit.y * hit.y + hit.z * hit.z);
        if (dist < cam->distance) {
            cam->distance = dist;
            /* eye = start + hit (w = start.w), staged through newEye */
            newEye.x = desc->start.x + hit.x;
            newEye.y = desc->start.y + hit.y;
            newEye.z = desc->start.z + hit.z;
            newEye.w = desc->start.w;
            cam->base.eye[0] = newEye.x;
            cam->base.eye[1] = newEye.y;
            cam->base.eye[2] = newEye.z;
            cam->base.eye[3] = newEye.w;
            cam->base.eye[1] = cam->base.eye[1] - 8.0f;
            moved = 1;
            cam->wallAvoidTimer = 30;
        }
        /* eyeOfs = eye - target (w = eye.w), staged through tmp */
        tmp.x = cam->base.eye[0] - cam->base.target[0];
        tmp.y = cam->base.eye[1] - cam->base.target[1];
        tmp.z = cam->base.eye[2] - cam->base.target[2];
        tmp.w = cam->base.eye[3];
        eyeOfs = tmp;
    }

    if (moved == 0) {
        if (cam->wallAvoidTimer > 0 && goalMoving == 0) {
            cam->wallAvoidTimer = cam->wallAvoidTimer - 1;
        } else {
            timer = 0;
            if (cam->wallAvoidTimer <= 0) {
                timer = cam->wallAvoidTimer;
            }
            cam->wallAvoidTimer = timer;
            timer = timer - 1;
            cam->wallAvoidTimer = timer;
            ramp = (float)-timer * 0.033333335f;
            /* blend = clamp(ramp, 0, 1) (vmin with S733, vmax with S713; ramp is never NaN) */
            blend = (ramp < 1.0f) ? ramp : 1.0f;
            blend = (blend > 0.0f) ? blend : 0.0f;

            /* dir = normalize(dir) * default distance */
            d2 = desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z;
            r = (d2 == 0.0f) ? 0.0f : VfRsq(d2);
            r = r * g_gameFieldCameraDefaultDistance;
            desc->dir.x = desc->dir.x * r;
            desc->dir.y = desc->dir.y * r;
            desc->dir.z = desc->dir.z * r;
            desc->dir.w = 0.0f;
            dist = g_gameFieldCameraDefaultDistance;

            if (CollisionRaycast(0x37bf0700, desc->shapeBlock, 1) != NULL) {
                t = g_collisionHitInfo.bestT;
                hit2.x = desc->dir.x * t;
                hit2.y = desc->dir.y * t;
                hit2.z = desc->dir.z * t;
                hit2.w = 0.0f;
                d2 = desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z;
                r = (d2 == 0.0f) ? 0.0f : VfRsq(d2);
                r = r * 4.0f;
                desc->dir.x = desc->dir.x * r;
                desc->dir.y = desc->dir.y * r;
                desc->dir.z = desc->dir.z * r;
                desc->dir.w = 0.0f;
                hit2.x = hit2.x + desc->dir.x;
                hit2.y = hit2.y + desc->dir.y;
                hit2.z = hit2.z + desc->dir.z;
                len = __builtin_sqrtf(hit2.x * hit2.x + hit2.y * hit2.y + hit2.z * hit2.z);
                if (!(dist <= len)) {
                    dist = __builtin_sqrtf(hit2.x * hit2.x + hit2.y * hit2.y + hit2.z * hit2.z);
                }
            }

            /* vcos.s of (blend * π) * S703: the quarter-turn scale cancels */
            c = __builtin_cosf(blend * 3.14159274f);
            step = (1.0f - c) * 0.5f * (dist - cam->distance) * 0.1f;
            if (goalMoving != 0) {
                step = cam->distance;
                lim = __builtin_sqrtf(goalDist2);
                step = dist - step;
                if (lim < step) {
                    step = lim;
                }
            }
            cam->distance = cam->distance + step;
        }
        /* dist = |eyeOfs| */
        dist = __builtin_sqrtf(eyeOfs.x * eyeOfs.x + eyeOfs.y * eyeOfs.y + eyeOfs.z * eyeOfs.z);
    }

    if (dist < 7.8f) {
        cam->base.nearZ = dist * 0.3f;
    } else {
        cam->base.nearZ = 3.9f;
    }
}
