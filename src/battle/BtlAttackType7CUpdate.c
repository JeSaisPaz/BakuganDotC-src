// bdc 0x0888261c BtlAttackType7CUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x7c (entry 124 of the handler table run by
   `BtlAttackUpdate`): a rising seeker that rains child attacks. Phase 0, frame 0: aims `dir` at
   the target aim point (`BtlAttackGetTargetAimPoint`; zero length: zero dir), sets `vel` =
   100 x dir, points its attached effects at -dir (`GfxEffectSetDirAttached`) and clears the
   curve `paramF0`. Frames 1..30: clash test (`BtlAttackCheckClash`); until the first hit it
   sweeps along `vel` (hit kind 0x9f) and on a hit sets `param0`, plays sound 0x200099 and spawns
   effect 0x1b at the hit point. From frame 9 on, `dir` bends toward `g_vecUp` by `paramF0`
   (which grows by 0.004 per frame), the effects follow -dir and `vel` = 100 x dir. It spawns five
   trail effects 0x97 spaced 0.1 x vel from its position and advances the position by `vel`.
   Frame 31+: stops its seeker effects (`BtlAttackStopSeekerEffects`), stores the aim point in
   `dir`, spawns effect 0x104 there and enters phase 1. Phase 1: each frame launches a child
   attack of type 0x7d (`BtlAttackCtor` on a 0x160-byte low-heap block, `BtlAttackLaunchAuto`
   pointing down `g_vecDown`) at the stored point plus a random offset in the xz disc of radius
   400 (angle `CoreRandFloat(6.28)`, distance `sqrt(CoreRandFloat(160000))`); every fourth frame,
   when the target (`BtlFindBakuganById`) is within 400 of the point in xz, it drops exactly on
   the target's x/z instead. After frame 80 it ends (`BtlAttackEnd`). The `w` lane of `dir` (frame 0), `vel` and the trail
   step is the bank zero S713 (masked lane of the C710 result). */

void BtlAttackType7CUpdate(BtlAttack *self)
{
    float trail[4];
    float step[4];
    float delta[4];
    float aim[4];
    float aimAt[4];
    float effectDir[4];
    float spawnPos[4];
    BtlBakugan *target;
    BtlAttack *child;
    GfxEffectMgr *mgr;
    float *pos;
    float angle;
    float dist;
    float dx;
    float dz;
    float distSq;
    float lenSq;
    float scale;
    bool fromLow;
    int i;

    if (self->phase == 1) {
        /* spawnPos = dir (stored drop point) */
        spawnPos[0] = self->dir[0];
        spawnPos[1] = self->dir[1];
        spawnPos[2] = self->dir[2];
        spawnPos[3] = self->dir[3];
        angle = CoreRandFloat(6.28000021f);
        dist = __builtin_sqrtf(CoreRandFloat(160000.0f));
        spawnPos[0] = spawnPos[0] + __builtin_sinf(angle) * dist;
        spawnPos[2] = spawnPos[2] + __builtin_cosf(angle) * dist;
        if (self->age % 4 == 0) {
            target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
            if (target != NULL) {
                /* squared xz distance between the drop point and the target (y lane zeroed) */
                dx = self->dir[0] - target->base.pos[0];
                dz = self->dir[2] - target->base.pos[2];
                distSq = dx * dx + dz * dz;
                if (distSq < 160000.0f) {
                    spawnPos[0] = target->base.pos[0];
                    spawnPos[2] = target->base.pos[2];
                }
            }
        }
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        child = MemAlloc(0x160, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (child != NULL) {
            BtlAttackCtor(child, self->owner, 0x7d);
        }
        BtlAttackLaunchAuto(child, spawnPos, &g_vecDown.x, NULL);
        if (self->age > 80) {
            BtlAttackEnd(self);
        }
        return;
    }
    if (self->phase != 0) {
        return;
    }

    pos = self->pos;
    if (self->age == 0) {
        BtlAttackGetTargetAimPoint(self, aim);
        /* dir = aim - pos (w = aim.w), then normalised and clamped to [-1, 1] with w = 0 */
        delta[0] = aim[0] - pos[0];
        delta[1] = aim[1] - pos[1];
        delta[2] = aim[2] - pos[2];
        delta[3] = aim[3];
        self->dir[0] = delta[0];
        self->dir[1] = delta[1];
        self->dir[2] = delta[2];
        self->dir[3] = delta[3];
        lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1] + self->dir[2] * self->dir[2];
        scale = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            scale = 0.0f;
        }
        self->dir[0] = VfSat1(self->dir[0] * scale);
        self->dir[1] = VfSat1(self->dir[1] * scale);
        self->dir[2] = VfSat1(self->dir[2] * scale);
        self->dir[3] = 0.0f;
        /* vel = 100 x dir (w = 0), staged through aim */
        aim[0] = self->dir[0] * 100.0f;
        aim[1] = self->dir[1] * 100.0f;
        aim[2] = self->dir[2] * 100.0f;
        aim[3] = 0.0f;
        self->vel[0] = aim[0];
        self->vel[1] = aim[1];
        self->vel[2] = aim[2];
        self->vel[3] = aim[3];
        /* aim = -dir for the attached effects (w kept) */
        aim[0] = -self->dir[0];
        aim[1] = -self->dir[1];
        aim[2] = -self->dir[2];
        aim[3] = self->dir[3];
        GfxEffectSetDirAttached(g_btlAttackEffectMgr, -1, aim, pos);
        self->paramF0 = 0.0f;
        return;
    }
    if (self->age >= 31) {
        BtlAttackStopSeekerEffects(g_btlAttackEffectMgr, pos);
        BtlAttackGetTargetAimPoint(self, aimAt);
        self->dir[0] = aimAt[0];
        self->dir[1] = aimAt[1];
        self->dir[2] = aimAt[2];
        self->dir[3] = aimAt[3];
        mgr = g_btlAttackEffectMgr;
        BtlAttackGetTargetAimPoint(self, aimAt);
        GfxEffectSpawn(mgr, 0x104, aimAt);
        self->phase++;
        return;
    }

    BtlAttackCheckClash(self);
    if (self->param0 == 0 &&
        BtlAttackSweepHit(self->radius, self, pos, self->vel, 0x9f, 3, 0, 0x31bf337e) != 0) {
        self->param0 = 1;
        BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, &g_btlAttackHitPoint.x);
    }
    if (self->age >= 9) {
        /* dir += (up - dir) x paramF0 (all four lanes); effectDir = -dir (w kept) */
        self->dir[0] = self->dir[0] + (g_vecUp.x - self->dir[0]) * self->paramF0;
        self->dir[1] = self->dir[1] + (g_vecUp.y - self->dir[1]) * self->paramF0;
        self->dir[2] = self->dir[2] + (g_vecUp.z - self->dir[2]) * self->paramF0;
        self->dir[3] = self->dir[3] + (g_vecUp.w - self->dir[3]) * self->paramF0;
        effectDir[0] = -self->dir[0];
        effectDir[1] = -self->dir[1];
        effectDir[2] = -self->dir[2];
        effectDir[3] = self->dir[3];
        GfxEffectSetDirAttached(g_btlAttackEffectMgr, -1, effectDir, pos);
        /* vel = 100 x dir (w = 0), staged through effectDir */
        effectDir[0] = self->dir[0] * 100.0f;
        effectDir[1] = self->dir[1] * 100.0f;
        effectDir[2] = self->dir[2] * 100.0f;
        effectDir[3] = 0.0f;
        self->vel[0] = effectDir[0];
        self->vel[1] = effectDir[1];
        self->vel[2] = effectDir[2];
        self->vel[3] = effectDir[3];
        self->paramF0 = self->paramF0 + 0.00400000019f;
    }
    /* trail = pos; step = 0.1 x vel (w = 0) */
    trail[0] = pos[0];
    trail[1] = pos[1];
    trail[2] = pos[2];
    trail[3] = pos[3];
    step[0] = self->vel[0] * 0.100000001f;
    step[1] = self->vel[1] * 0.100000001f;
    step[2] = self->vel[2] * 0.100000001f;
    step[3] = 0.0f;
    for (i = 0; i < 5; i++) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x97, trail);
        trail[0] = trail[0] + step[0];
        trail[1] = trail[1] + step[1];
        trail[2] = trail[2] + step[2];
    }
    /* pos.xyz += vel.xyz (w kept) */
    pos[0] = pos[0] + self->vel[0];
    pos[1] = pos[1] + self->vel[1];
    pos[2] = pos[2] + self->vel[2];
}
