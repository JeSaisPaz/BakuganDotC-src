// bdc 0x088b5c50 ActorStageObjDebrisLaunchStrong
#include "bdc.h"

/* Stronger variant of `ActorStageObjDebrisLaunch` used by `ActorCrystalBreak` (the big crystal
   actor). Resets the debris model (`fade` 1, `lifetime` 180, `activeMask` all set, ambient
   `g_colorWhite`, `launchVel` zero), loads `mtx` as the model root matrix and poses it
   (`GfxModelUpdateAndApplyMotion`), stores `vel` (when given) as `launchVel` and adds 3 to its Y,
   allocates `fragLast` and one `CollisionPhysBox` per model part (`CxxVecNew`). Each part's
   node matrix is moved to world space (root x node), gets a physics box from its part bounds with
   floor `floorY - centre Y of the bounds` and gravity 0.6, an upward impulse (`g_vecUp`, 0.5), a
   velocity of the outward direction from `(rootX, centerY - 1, rootZ)` scaled to `min(dist, 300)`
   (zero for a zero-length direction) plus a random X/Z spread in [-6, 6) (`g_debrisRandCentre`,
   `g_debrisStrongSpread`), plus `launchVel`, and a random-corner nudge of 30 x the outward
   vector. Finally resets the root matrix to identity and makes the model visible. */

void ActorStageObjDebrisLaunchStrong(float floorY, float centerY, ActorStageObjDebris *debris,
                                     const float *mtx, const ScePspFVector4 *vel)
{
    ActorStageObjDebris *self = debris;
    ScePspFVector4 centre;
    ScePspFVector4 dir BDC_ALIGN16;
    ScePspFVector4 rnd;
    ScePspFVector4 bounds[2] BDC_ALIGN16;
    ScePspFVector4 tmp BDC_ALIGN16;
    float prod[16];
    const float *root;
    float *node;
    GmoNode *nodeObj;
    bool fromLow;
    s32 size;
    s32 count;
    void *block;
    s32 i;
    s32 j;
    s32 r;
    float dist;
    float lenSq;
    float k;

    self->base.base.unk08 = 0x86;
    self->fade = 1.0f;
    self->frame = 0;
    self->fragments = NULL;
    self->activeMask = 0xffffffff;
    self->base.lighting = 1;
    self->base.ambient[0] = g_colorWhite.x;
    self->base.ambient[1] = g_colorWhite.y;
    self->base.ambient[2] = g_colorWhite.z;
    self->base.ambient[3] = g_colorWhite.w;
    memset(self->settleFrames, 0, sizeof(self->settleFrames));
    self->unk190 = 1.0f;
    self->unk194 = 1.5f;
    self->flag198 = 0;
    self->lifetime = 180;
    /* C720 of the VFPU constant bank: zero vector */
    self->launchVel.x = 0.0f;
    self->launchVel.y = 0.0f;
    self->launchVel.z = 0.0f;
    self->launchVel.w = 0.0f;
    self->unk1c4 = 0;
    self->impactSoundId = 0;
    self->flag1d0 = 1;
    for (j = 0; j < 16; j++) {
        self->base.data->rootMatrix[j] = mtx[j];
    }
    GfxModelUpdateAndApplyMotion(&self->base);
    if (vel != NULL) {
        self->launchVel = *vel;
    }
    self->launchVel.y = self->launchVel.y + 3.0f;

    size = self->base.partCount * (s32)sizeof(s32);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    count = self->base.partCount;
    self->fragLast = block;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(count * (s32)sizeof(CollisionPhysBox) + 0x10 /* PSP: CxxVecNew array cookie */, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (block != NULL) {
        self->fragments = CxxVecNew((u8 *)block + g_cxxVecCookieSize, count,
                                    sizeof(CollisionPhysBox), CollisionPhysBoxCtor, 0);
    } else {
        self->fragments = NULL;
    }

    root = self->base.data->rootMatrix;
    centre.x = root[12];
    centre.y = centerY - 1.0f;
    centre.z = root[14];
    centre.w = 0.0f;
    if (g_debrisStrongSpreadInit == 0) {
        g_debrisStrongSpreadInit = 1;
        g_debrisStrongSpread.x = 12.0f;
        g_debrisStrongSpread.y = 0.0f;
        g_debrisStrongSpread.z = 12.0f;
        g_debrisStrongSpread.w = 0.0f;
    }

    for (i = 0; i < self->base.partCount; i++) {
        nodeObj = GfxModelFindNode(&self->base, GfxModelGetPartName(&self->base, i));
        node = nodeObj->localMatrix;
        GfxModelGetPartBounds(&self->base, i, &bounds[0].x);
        /* node matrix = root matrix x node matrix (column-major, columns are 4-float groups) */
        root = self->base.data->rootMatrix;
        for (j = 0; j < 4; j++) {
            for (r = 0; r < 4; r++) {
                prod[j * 4 + r] = node[j * 4 + 0] * root[0 + r] + node[j * 4 + 1] * root[4 + r] +
                                  node[j * 4 + 2] * root[8 + r] + node[j * 4 + 3] * root[12 + r];
            }
        }
        for (j = 0; j < 16; j++) {
            node[j] = prod[j];
        }
        CollisionPhysBoxInit(&self->fragments[i], node, bounds, false);
        self->fragments[i].floorY = floorY - (bounds[1].y + bounds[0].y) * 0.5f;
        self->fragments[i].gravity = 0.600000024f;
        CollisionPhysBoxApplyImpulse(0.5f, &self->fragments[i], &g_vecUp.x, NULL);

        /* dir = node position - centre (xyz, w = node w); dist = |dir| */
        dir.x = node[12] - centre.x;
        dir.y = node[13] - centre.y;
        dir.z = node[14] - centre.z;
        dir.w = node[15];
        dist = __builtin_sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        if (!(dist <= 300.0f)) {
            dist = 300.0f;
        }
        /* dir = normalise(dir) * dist; factor 0 (bank S713) when |dir| is zero; w = S713 = 0 */
        lenSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
        if (lenSq == 0.0f) {
            k = 0.0f;
        } else {
            k = VfRsq(lenSq);
        }
        k = k * dist;
        dir.x = dir.x * k;
        dir.y = dir.y * k;
        dir.z = dir.z * k;
        dir.w = 0.0f;
        if (g_debrisRandCentreInit == 0) {
            g_debrisRandCentreInit = 1;
            g_debrisRandCentre.x = 1.5f;
            g_debrisRandCentre.y = 1.5f;
            g_debrisRandCentre.z = 1.5f;
            g_debrisRandCentre.w = 0.0f;
        }
        /* rnd = (random [1, 2) - 1.5) * spread (vrndf1.t); tmp = dir + rnd (xyz) */
        rnd.x = PlatformRandFloat12();
        rnd.y = PlatformRandFloat12();
        rnd.z = PlatformRandFloat12();
        rnd.x = rnd.x - g_debrisRandCentre.x;
        rnd.y = rnd.y - g_debrisRandCentre.y;
        rnd.z = rnd.z - g_debrisRandCentre.z;
        rnd.w = 0.0f;
        rnd.x = rnd.x * g_debrisStrongSpread.x;
        rnd.y = rnd.y * g_debrisStrongSpread.y;
        rnd.z = rnd.z * g_debrisStrongSpread.z;
        tmp.x = dir.x + rnd.x;
        tmp.y = dir.y + rnd.y;
        tmp.z = dir.z + rnd.z;
        tmp.w = dir.w;
        CollisionPhysBoxAddVelocity(&self->fragments[i], &tmp.x);
        CollisionPhysBoxAddVelocity(&self->fragments[i], &self->launchVel.x);
        /* tmp = dir * 30 (xyz), w = S713 = 0 */
        tmp.x = dir.x * 30.0f;
        tmp.y = dir.y * 30.0f;
        tmp.z = dir.z * 30.0f;
        tmp.w = 0.0f;
        CollisionPhysBoxNudgeRandomCorner(&self->fragments[i], &tmp.x);
        self->fragLast[i] = 0;
        self->contactFlag[i] = 0;
    }

    /* root matrix = identity (vmidt.q) */
    for (j = 0; j < 16; j++) {
        self->base.data->rootMatrix[j] = (j % 5 == 0) ? 1.0f : 0.0f;
    }
    self->base.visible = 1;
}
