// bdc 0x0888ec5c BtlAiRaycastBlocked
#include "bdc.h"

/* Line-of-sight test of the CPU AI object (0xa30 bytes, `BtlAiCreate`): casts from `from` to `to`
   against the stage obstacles and units (`ActorStageObjRaycastAll`) and the stage colliders
   (`CollisionRaycast` with layer mask `0x3fbf2700` on `g_collisionSegmentDesc` /
   `g_collisionSegmentBlock`); returns the kind of what blocks the ray (the
   `ActorStageObjRaycastAll` kind 1..4, or `0xff` when a stage collider is hit nearer than the
   obstacle, whose distance is first scaled by 0.8), 0 when clear or when the AI has no owner, and
   optionally writes the hit distance to `*hit` (0 first). When the hit is kind 2 or a stage
   collider, it raises `from[1]` (written back) to head height (ground + stat height + clearance)
   and, if that is below the ceiling, re-casts the collider segment from there and sets
   `rayHitsSoft` when that ray is clear. */

int BtlAiRaycastBlocked(BtlAi *self, float *from, float *to, float *hit)
{
    float origin[4];
    float delta[4];
    float start[4];
    float dx;
    float dy;
    float dz;
    float t;
    float dist;
    float headY;
    SegmentShape *segment = &g_collisionSegmentDesc;
    const VtblEntry *update;
    BtlBakugan *owner;
    s32 kind;
    s32 recast;

    kind = 0;
    self->rayHitsSoft = 0;
    if (hit != NULL) {
        *hit = 0.0f;
    }
    recast = 0;
    if (self->owner == NULL) {
        return kind;
    }

    /* Obstacle / unit cast from the high ray height. */
    start[0] = from[0];
    start[1] = from[1];
    start[2] = from[2];
    start[3] = from[3];
    start[1] = start[1] + g_btlAiRangeConsts.rayHighHeight;
    origin[0] = start[0];
    origin[1] = start[1];
    origin[2] = start[2];
    origin[3] = start[3];
    delta[0] = to[0];
    delta[1] = to[1];
    delta[2] = to[2];
    delta[3] = to[3];
    kind = ActorStageObjRaycastAll(origin, delta, &t);
    if (kind == 0) {
        t = g_btlAiFloatInf;
    } else {
        if (kind == 2) {
            recast = 1;
        }
        if (hit != NULL) {
            *hit = t;
        }
    }

    /* Stage collider cast from the low ray height. */
    start[0] = from[0];
    start[1] = from[1];
    start[2] = from[2];
    start[3] = from[3];
    start[1] = start[1] + g_btlAiRangeConsts.rayLowHeight;
    segment->start[0] = start[0];
    segment->start[1] = start[1];
    segment->start[2] = start[2];
    segment->start[3] = start[3];
    segment->dir[0] = to[0];
    segment->dir[1] = to[1];
    segment->dir[2] = to[2];
    segment->dir[3] = to[3];
    update = &((const VtblEntry *)segment->info)[9];
    ((void (*)(void *))update->fn)((u8 *)segment + update->delta);
    if (CollisionRaycast(0x3fbf2700, &g_collisionSegmentBlock, 0) != NULL) {
        /* |start - hitPoint| over x, y, z. */
        dx = start[0] - g_collisionHitResult.point.x;
        dy = start[1] - g_collisionHitResult.point.y;
        dz = start[2] - g_collisionHitResult.point.z;
        dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        if (kind != 0) {
            t = t * 0.8f;
        }
        if (dist < t) {
            recast = 2;
            if (hit != NULL) {
                *hit = dist;
            }
            kind = 0xff;
        }
    }

    if (recast != 0) {
        owner = self->owner;
        headY = owner->groundPoint[1] + owner->combat.stats->height + g_btlAiRangeConsts.rayHeadClearance;
        from[1] = headY;
        if (headY < BtlStageGetCeilingHeight()) {
            segment->start[0] = from[0];
            segment->start[1] = from[1];
            segment->start[2] = from[2];
            segment->start[3] = from[3];
            update = &((const VtblEntry *)segment->info)[9];
            ((void (*)(void *))update->fn)((u8 *)segment + update->delta);
            if (CollisionRaycast(0x3fbf2700, &g_collisionSegmentBlock, 0) == NULL) {
                self->rayHitsSoft = 1;
            }
        }
    }
    return kind;
}
