// bdc 0x088b5708 ActorStageObjDebrisLaunch
#include "bdc.h"

/* Launches the fragments of a break-debris model (`ActorStageObjDebrisCtor`): resets its state
   (`unk08 = 0x86`, `fade = 1`, `frame = 0`, `activeMask = -1`, lighting on, ambient white
   `g_colorWhite`, `settleFrames` cleared, `lifetime = 180`, `launchVel` zero), copies the source
   object's matrix `mtx` into the model root matrix and poses it (`GfxModelUpdateAndApplyMotion`),
   copies `vel` into `launchVel` when not NULL, allocates `fragLast` (`partCount` words) and
   `fragments` (`partCount` physics boxes via `CxxVecNew`/`CollisionPhysBoxCtor`; NULL when the
   allocation fails). Each part node gets root x node as its matrix, a physics box from its part
   bounds (`CollisionPhysBoxInit`), floor `floorY` minus the mid height of the bounds, gravity 2.3,
   an up impulse (`g_vecUp`, 0.5) and a velocity pushing it away from
   `(root.x, centerY - 1, root.z)` (speed `min(dist, 300) * 0.05`, zero for a zero-length push, plus
   a random term `(rand[1,2) - g_debrisRandCentre) * g_debrisSpread`, plus `launchVel`, plus a
   random-corner nudge of 5 x the push). Finally resets the root matrix to identity and makes the
   model visible. */

void ActorStageObjDebrisLaunch(float floorY, float centerY, void *debris, float *mtx, float *vel)
{
    ActorStageObjDebris *self = (ActorStageObjDebris *)debris;
    float centre[4] __attribute__((aligned(16)));
    float push[4] __attribute__((aligned(16)));
    float rnd[4] __attribute__((aligned(16)));
    ScePspFVector4 bounds[2] __attribute__((aligned(16)));
    float tmp[4] __attribute__((aligned(16)));
    float prod[16];
    float *root;
    bool fromLow;
    s32 count;
    void *block;
    CollisionPhysBox *boxes;
    GmoNode *node;
    float dist;
    float lenSq;
    float scale;
    s32 i;
    s32 j;
    s32 k;
    s32 r;

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
    /* bank C720 = (0, 0, 0, 0) */
    self->launchVel.x = 0.0f;
    self->launchVel.y = 0.0f;
    self->launchVel.z = 0.0f;
    self->launchVel.w = 0.0f;
    self->unk1c4 = 0;
    self->impactSoundId = 0;
    self->flag1d0 = 1;
    root = self->base.data->rootMatrix;
    for (k = 0; k < 16; k++) {
        root[k] = mtx[k];
    }
    GfxModelUpdateAndApplyMotion(&self->base);
    if (vel != NULL) {
        self->launchVel.x = vel[0];
        self->launchVel.y = vel[1];
        self->launchVel.z = vel[2];
        self->launchVel.w = vel[3];
    }

    count = self->base.partCount;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(count * (s32)sizeof(s32), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    count = self->base.partCount;
    self->fragLast = (s32 *)block;
    boxes = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(count * (s32)sizeof(CollisionPhysBox) + 0x10 /* PSP: CxxVecNew array cookie */, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (block != NULL) {
        boxes = CxxVecNew((u8 *)block + g_cxxVecCookieSize, count, sizeof(CollisionPhysBox),
                          CollisionPhysBoxCtor, 0);
    }
    self->fragments = boxes;

    centre[0] = self->base.data->rootMatrix[12];
    centre[1] = centerY - 1.0f;
    centre[2] = self->base.data->rootMatrix[14];
    centre[3] = 0.0f;
    if (g_debrisSpreadInit == 0) {
        g_debrisSpreadInit = 1;
        g_debrisSpread.x = 5.0f;
        g_debrisSpread.y = 0.0f;
        g_debrisSpread.z = 5.0f;
        g_debrisSpread.w = 0.0f;
    }

    for (i = 0; i < self->base.partCount; i++) {
        node = (GmoNode *)GfxModelFindNode(&self->base, GfxModelGetPartName(&self->base, i));
        GfxModelGetPartBounds(&self->base, i, &bounds[0].x);
        /* node matrix = root matrix x node matrix (column-major) */
        root = self->base.data->rootMatrix;
        for (j = 0; j < 4; j++) {
            for (r = 0; r < 4; r++) {
                prod[j * 4 + r] = node->localMatrix[j * 4 + 0] * root[0 * 4 + r]
                                + node->localMatrix[j * 4 + 1] * root[1 * 4 + r]
                                + node->localMatrix[j * 4 + 2] * root[2 * 4 + r]
                                + node->localMatrix[j * 4 + 3] * root[3 * 4 + r];
            }
        }
        for (k = 0; k < 16; k++) {
            node->localMatrix[k] = prod[k];
        }
        CollisionPhysBoxInit(&self->fragments[i], node->localMatrix, bounds, false);
        self->fragments[i].floorY = floorY - (bounds[1].y + bounds[0].y) * 0.5f;
        self->fragments[i].gravity = 2.29999995f;
        CollisionPhysBoxApplyImpulse(0.5f, &self->fragments[i], &g_vecUp.x, NULL);

        /* push = node position - centre; dist = |push| */
        tmp[0] = node->localMatrix[12] - centre[0];
        tmp[1] = node->localMatrix[13] - centre[1];
        tmp[2] = node->localMatrix[14] - centre[2];
        tmp[3] = node->localMatrix[15];
        push[0] = tmp[0];
        push[1] = tmp[1];
        push[2] = tmp[2];
        push[3] = tmp[3];
        dist = __builtin_sqrtf(push[0] * push[0] + push[1] * push[1] + push[2] * push[2]);
        if (!(dist <= 300.0f)) {
            dist = 300.0f;
        }
        dist = dist * 0.05f;
        /* push = normalize(push) * speed; bank S713 = 0 for a zero-length push */
        lenSq = push[0] * push[0] + push[1] * push[1] + push[2] * push[2];
        scale = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        scale = scale * dist;
        push[0] = push[0] * scale;
        push[1] = push[1] * scale;
        push[2] = push[2] * scale;
        push[3] = 0.0f;
        if (g_debrisRandCentreInit == 0) {
            g_debrisRandCentreInit = 1;
            g_debrisRandCentre.x = 1.5f;
            g_debrisRandCentre.y = 1.5f;
            g_debrisRandCentre.z = 1.5f;
            g_debrisRandCentre.w = 0.0f;
        }
        /* rnd = (random [1,2) - g_debrisRandCentre) * g_debrisSpread; w from bank S713 = 0 */
        rnd[0] = PlatformRandFloat12();
        rnd[1] = PlatformRandFloat12();
        rnd[2] = PlatformRandFloat12();
        rnd[0] = rnd[0] - g_debrisRandCentre.x;
        rnd[1] = rnd[1] - g_debrisRandCentre.y;
        rnd[2] = rnd[2] - g_debrisRandCentre.z;
        rnd[3] = 0.0f;
        rnd[0] = rnd[0] * g_debrisSpread.x;
        rnd[1] = rnd[1] * g_debrisSpread.y;
        rnd[2] = rnd[2] * g_debrisSpread.z;
        tmp[0] = push[0] + rnd[0];
        tmp[1] = push[1] + rnd[1];
        tmp[2] = push[2] + rnd[2];
        tmp[3] = push[3];
        CollisionPhysBoxAddVelocity(&self->fragments[i], tmp);
        CollisionPhysBoxAddVelocity(&self->fragments[i], &self->launchVel.x);
        tmp[0] = push[0] * 5.0f;
        tmp[1] = push[1] * 5.0f;
        tmp[2] = push[2] * 5.0f;
        tmp[3] = 0.0f;
        CollisionPhysBoxNudgeRandomCorner(&self->fragments[i], tmp);
        self->fragLast[i] = 0;
        self->contactFlag[i] = 0;
    }

    root = self->base.data->rootMatrix;
    for (k = 0; k < 16; k++) {
        root[k] = (k % 5 == 0) ? 1.0f : 0.0f;
    }
    self->base.visible = 1;
}
