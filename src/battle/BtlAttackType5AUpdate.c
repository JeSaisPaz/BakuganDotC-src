// bdc 0x08880fe8 BtlAttackType5AUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x5a (ground strike). Once age > 20 it stops the attached
   effects 0x26a/0x26b at its position and ends. On frame 0 it scales vel.xyz by 200, clears its
   effect and sets the battle main task's flash target to 2; at age 8 the flash target goes back
   to 0. Otherwise it sweeps pos along vel (BtlAttackSweepHit, hit kind 0x7d): on a hit it spawns
   effect 0x7f at the hit point, stops 0x26a/0x26b, ends and (age < 9) clears the flash target;
   on a miss it walks n = (s32)(|vel| * 0.03 + 0.5) steps of vel / (|vel| * 0.03 + 0.5) from pos,
   raycasting down at each (CollisionRaycastPoint) and spawning directed effect 0xf5 at a ground
   hit higher than 200 below the step point, then advances pos by vel and damps vel.xyz by 0.9. Both vel scalings store the vscl.t result
   column C710 whole, so vel.w becomes the bank zero S713 (0.0f). */

void BtlAttackType5AUpdate(BtlAttack *self)
{
    float point[4];
    float step[4];
    float ground[4];
    float *pos;
    float *vel;
    float len;
    float count;
    float inv;
    s32 n;
    s32 i;

    if (self->age > 20) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x26a, self->pos);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x26b, self->pos);
        BtlAttackEnd(self);
        return;
    }
    vel = self->vel;
    if (self->age == 0) {
        /* vel.xyz *= 200; w = S713 (0) */
        vel[0] = vel[0] * 200.0f;
        vel[1] = vel[1] * 200.0f;
        vel[2] = vel[2] * 200.0f;
        vel[3] = 0.0f;
        self->effect = NULL;
        if (BtlCameraTaskExists() != 0) {
            ((BtlMain *)BtlGetCameraTask())->flashTarget = 2.0f;
        }
        return;
    }
    pos = self->pos;
    if (self->age == 8 && BtlCameraTaskExists() != 0) {
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
    }
    if (BtlAttackSweepHit(self->radius, self, pos, vel, 0x7d, 3, 0, 0x31bf337e) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x7f, &g_btlAttackHitPoint.x);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x26a, pos);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x26b, pos);
        BtlAttackEnd(self);
        if (self->age < 9) {
            ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
        }
        return;
    }
    /* point = pos, step = vel, len = |vel.xyz| */
    for (i = 0; i < 4; i++) {
        point[i] = pos[i];
        step[i] = vel[i];
    }
    len = __builtin_sqrtf(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
    count = len * 0.0299999993f + 0.5f;
    n = (s32)count;
    /* step.xyz *= 1 / count; w = S713 (0) */
    inv = 1.0f / count;
    step[0] = step[0] * inv;
    step[1] = step[1] * inv;
    step[2] = step[2] * inv;
    step[3] = 0.0f;
    for (i = 0; i < n; i++) {
        if (CollisionRaycastPoint(point, ground) != 0 && point[1] - 200.0f < ground[1]) {
            GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xf5, ground, self->dir);
        }
        /* point.xyz += step.xyz */
        point[0] = point[0] + step[0];
        point[1] = point[1] + step[1];
        point[2] = point[2] + step[2];
    }
    /* pos.xyz += vel.xyz (w kept); vel.xyz *= 0.9, w = S713 (0) */
    pos[0] = pos[0] + vel[0];
    pos[1] = pos[1] + vel[1];
    pos[2] = pos[2] + vel[2];
    vel[0] = vel[0] * 0.899999976f;
    vel[1] = vel[1] * 0.899999976f;
    vel[2] = vel[2] * 0.899999976f;
    vel[3] = 0.0f;
}
