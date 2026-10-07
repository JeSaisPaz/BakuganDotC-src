// bdc 0x08855a18 ActorCrystalCtor
#include "bdc.h"

/* Constructor of the 0xa90-byte crystal actor (model `fz_crystal01.gmo`, id 0x21): a subclass of
   the battle unit class — it runs `BtlBakuganCtor` first, then installs `g_actorCrystalVtable`
   and the shape descriptors of `extraCapsule0`, `modelBox`, `pushShape` and `extraCapsule1`,
   scales the model by 0.6 (back to 1.0 with `barrierVariant = 1` when `flag == 1` on stage 10 in
   rule mode 1), sets the combat level 5 (`BtlCombatSetLevel`) or, in score mode
   (`BtlIsScoreMode`), 600 HP (`BtlCombatFillHp`), places the object at `pos` (outside score
   mode snapped to the ground with `CollisionRaycastPoint`; a miss below y = -500 resets y to
   0) and copies it to `homePos`, `pos`, the model root translation and `basePos`; builds the HP
   gauge (in battle), the colliders (`ActorCrystalCreateColliders`, layer 5), outside score
   mode the `<model>_stand_60.gmo` / `_stand.gmo` stand (`ActorCrystalStandCtor`, appended to
   `g_stageObjList`), resets the behaviour state (`ActorCrystalSetMode` 0), hooks the
   materials and the "spell"/"context00"/"context01" node matrices, picks the colour style (random
   in score mode, else `type`/`style`), places the three `orbitPoints` 700 units around the
   crystal at random angles, allocates the 0xc0-byte `collisionBox` from the model bbox and ends
   with `ActorCrystalUpdateMatrices`. Returns `self`.
   VFPU bank constants lifted as literals: S713 = 0 (`scale[3]`), C720 = 0 (`base.spawnPoint`,
   `warpPos`, `effectPos`, `attachPos`, the `modelBox` extents), S733 = 1 (subtracted from the
   vrndf1 random) and S703 = 2/pi (vrot angle scale). */

/* invTransform = inverse of the rigid `transform` of a box: transposed 3x3 rows (w lanes 0) and the
   negated rotated translation, w kept from the translation's w. */
static void CrystalBoxBuildInverse(CollisionBox *box)
{
    ScePspFMatrix4 *m = &box->transform;
    float tx;
    float ty;
    float tz;

    tx = m->x.x * m->w.x + m->x.y * m->w.y + m->x.z * m->w.z;
    ty = m->y.x * m->w.x + m->y.y * m->w.y + m->y.z * m->w.z;
    tz = m->z.x * m->w.x + m->z.y * m->w.y + m->z.z * m->w.z;
    box->invTransform.x.x = m->x.x;
    box->invTransform.x.y = m->y.x;
    box->invTransform.x.z = m->z.x;
    box->invTransform.x.w = 0.0f;
    box->invTransform.y.x = m->x.y;
    box->invTransform.y.y = m->y.y;
    box->invTransform.y.z = m->z.y;
    box->invTransform.y.w = 0.0f;
    box->invTransform.z.x = m->x.z;
    box->invTransform.z.y = m->y.z;
    box->invTransform.z.z = m->z.z;
    box->invTransform.z.w = 0.0f;
    box->invTransform.w.x = -tx;
    box->invTransform.w.y = -ty;
    box->invTransform.w.z = -tz;
    box->invTransform.w.w = m->w.w;
}

ActorCrystal *ActorCrystalCtor(ActorCrystal *self, s32 modelId, s32 mode, float *pos, s32 flag, s32 type, s32 style)
{
    float color[4];
    char name[128];
    void *mem;
    void *stand;
    CollisionBox *boxMem;
    CollisionBox *box;
    GmoModel *data;
    void *node;
    char *dot;
    bool fromLow;
    float *src;
    float r;
    float jitter;
    float angle;
    float px;
    float pz;
    float k;
    s32 i;
    s32 j;

    BtlBakuganCtor(&self->base, modelId);
    self->base.base.base.vtable = g_actorCrystalVtable;
    self->extraCapsule0.vtbl = g_collisionCapsuleVtbl;
    ((SegmentShape *)self->extraCapsule0.segmentHead)->info = (void *)g_collisionSegmentVtbl;
    ((SegmentShape *)self->extraCapsule0.segmentHead)->type = 2;
    self->extraCapsule0.type = 4;
    self->modelBox.vtbl = g_collisionBoxVtbl;
    self->modelBox.invValid = 0;
    self->modelBox.type = 6;
    self->pushShape.vtbl = g_collisionCapsuleVtbl;
    ((SegmentShape *)self->pushShape.segmentHead)->info = (void *)g_collisionSegmentVtbl;
    ((SegmentShape *)self->pushShape.segmentHead)->type = 2;
    self->pushShape.type = 4;
    self->extraCapsule1.vtbl = g_collisionCapsuleVtbl;
    ((SegmentShape *)self->extraCapsule1.segmentHead)->info = (void *)g_collisionSegmentVtbl;
    ((SegmentShape *)self->extraCapsule1.segmentHead)->type = 2;
    self->extraCapsule1.type = 4;
    self->base.base.base.unk08 = modelId;
    self->inBattle = BtlCameraTaskExists() != 0;
    self->spawnFlag = flag;
    self->barrierVariant = 0;
    /* scale.xyz *= 0.6; scale.w = bank S713 (0) */
    self->base.base.scale[0] = self->base.base.scale[0] * 0.6f;
    self->base.base.scale[1] = self->base.base.scale[1] * 0.6f;
    self->base.base.scale[2] = self->base.base.scale[2] * 0.6f;
    self->base.base.scale[3] = 0.0f;
    if (g_scriptGlobalVars[8] == 1 && g_scriptGlobalVars[1] == 10 && flag == 1) {
        self->base.base.scale[0] = 1.0f;
        self->base.base.scale[1] = 1.0f;
        self->base.base.scale[2] = 1.0f;
        self->base.base.scale[3] = 0.0f;
        self->barrierVariant = 1;
    }
    if (BtlIsScoreMode(1) != 0) {
        BtlCombatFillHp(&self->base.combat, 600.0f);
    } else {
        BtlCombatSetLevel(&self->base.combat, 5);
    }
    self->base.targetPointVariant = mode;
    if (BtlIsScoreMode(1) == 0) {
        if (CollisionRaycastPoint(pos, pos) == 0 && pos[1] < -500.0f) {
            pos[1] = 0.0f;
        }
    }
    /* pos -> homePos, pos -> model position */
    for (j = 0; j < 4; j++) {
        self->homePos[j] = pos[j];
    }
    for (j = 0; j < 4; j++) {
        self->base.base.pos[j] = pos[j];
    }
    self->base.base.rot[1] = pos[3];
    /* model position -> root matrix translation, -> basePos */
    for (j = 0; j < 4; j++) {
        self->base.base.data->rootMatrix[12 + j] = self->base.base.pos[j];
    }
    for (j = 0; j < 4; j++) {
        self->basePos[j] = self->base.base.pos[j];
    }
    if (self->inBattle) {
        BtlBakuganCreateHpGauge(&self->base);
    }
    self->base.collider0 = NULL;
    self->base.collider1 = NULL;
    self->collider2 = NULL;
    ActorCrystalCreateColliders(self);
    self->base.playerSlot = 5;
    BtlBakuganSetColliderLayer(&self->base, 5);
    self->stand = NULL;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            self->facingMatrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    if (BtlIsScoreMode(1) == 0) {
        strcpy(name, g_btlModelNames[modelId]);
        dot = strrchr(name, '.');
        if (dot != NULL) {
            *dot = '\0';
        }
        if (self->barrierVariant) {
            strcat(name, "_stand.gmo");
        } else {
            strcat(name, "_stand_60.gmo");
        }
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(0x150, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        stand = NULL;
        if (mem != NULL) {
            ActorCrystalStandCtor(mem, name);
            stand = mem;
        }
        self->stand = stand;
        /* homePos -> stand position -> stand root matrix translation */
        for (j = 0; j < 4; j++) {
            ((ActorCrystalStand *)self->stand)->base.pos[j] = self->homePos[j];
        }
        for (j = 0; j < 4; j++) {
            ((ActorCrystalStand *)self->stand)->base.data->rootMatrix[12 + j] =
                ((ActorCrystalStand *)self->stand)->base.pos[j];
        }
        ((ActorCrystalStand *)self->stand)->base.lighting = 1;
        CoreObjectListAppend((CoreObject *)self->stand, g_stageObjList);
    }
    self->step = 0;
    self->value8cc = 0;
    self->value8c4 = 0.0f;
    self->fadeSkip = 0;
    for (j = 0; j < 4; j++) {
        self->base.spawnPoint[j] = 0.0f;
    }
    self->value8d8 = 0;
    self->base.attackFrame = 0;
    self->record = NULL;
    self->value6c0 = 0;
    self->additive = 1;
    self->fireType = 2;
    self->shaking = 0;
    self->shakeFrame = 0;
    self->castStep = 0;
    self->hitByAll = 0;
    self->spellTotal = 0;
    self->spellCount = 0;
    self->cpuEntryDone = 0;
    self->mode2Done = 0;
    self->autoFire = 0;
    self->mode2Proceed = 0;
    for (j = 0; j < 4; j++) {
        self->warpPos[j] = 0.0f;
    }
    self->fadeTimer = 0;
    self->fadeStep = 0;
    self->fadeEnabled = 0;
    self->fadeOneShot = 0;
    self->fadeDone = 0;
    self->regenStep = 0;
    self->alpha = 1.0f;
    self->fade = 1.0f;
    for (j = 0; j < 4; j++) {
        self->effectPos[j] = 0.0f;
    }
    for (j = 0; j < 4; j++) {
        self->attachPos[j] = 0.0f;
    }
    self->base.unitTag = 2;
    if (!self->inBattle) {
        self->additive = 0;
    }
    if (GameStageIs4To7() != 0) {
        self->additive = 0;
    }
    self->auxObject = NULL;
    self->extraCapsule0.start[0] = 0.0f;
    self->extraCapsule0.start[1] = 0.0f;
    self->extraCapsule0.start[2] = 0.0f;
    self->extraCapsule0.axis[0] = 0.0f;
    self->extraCapsule0.axis[1] = 0.0f;
    self->extraCapsule0.axis[2] = 0.0f;
    self->extraCapsule0.axisLen = 0.0f;
    r = 0.0f;
    self->extraCapsule0.radius = r;
    self->extraCapsule0.radiusSq = r * r;
    self->extraCapsule0.axisLen = __builtin_sqrtf(self->extraCapsule0.axis[0] * self->extraCapsule0.axis[0] +
                                                  self->extraCapsule0.axis[1] * self->extraCapsule0.axis[1] +
                                                  self->extraCapsule0.axis[2] * self->extraCapsule0.axis[2]);
    /* modelBox extents = bank C720 (0) */
    data = self->base.base.data;
    self->modelBox.aabbMin.x = 0.0f;
    self->modelBox.aabbMin.y = 0.0f;
    self->modelBox.aabbMin.z = 0.0f;
    self->modelBox.aabbMin.w = 0.0f;
    self->modelBox.aabbMax.x = 0.0f;
    self->modelBox.aabbMax.y = 0.0f;
    self->modelBox.aabbMax.z = 0.0f;
    self->modelBox.aabbMax.w = 0.0f;
    self->modelBox.invValid = 0;
    self->modelBox.transform = *(ScePspFMatrix4 *)data->rootMatrix;
    if (!self->modelBox.invValid) {
        CrystalBoxBuildInverse(&self->modelBox);
        self->modelBox.invValid = 1;
    }
    for (i = 0; i < 3; i++) {
        self->spellKinds[i] = 0;
        self->spellTypes[i] = 0;
        self->spellPower[i] = 250.0f;
        self->spellParamA[i] = 0.0f;
        self->spellParamB[i] = 0.0f;
        self->spellParamC[i] = 0.0f;
    }
    self->autoCast = 0;
    self->scriptedCast = 0;
    self->onstageFlag = 0;
    self->scriptFlag = 0;
    self->valueA3c = 0;
    ActorCrystalSetMode(self, 0, false);
    ActorCrystalInitMaterials(self);
    node = GfxModelFindNode(&self->base.base, "spell");
    if (node != NULL) {
        self->nodeMatrices[0] = (ScePspFMatrix4 *)((GmoNode *)node)->localMatrix;
    }
    node = GfxModelFindNode(&self->base.base, "context00");
    if (node != NULL) {
        self->nodeMatrices[1] = (ScePspFMatrix4 *)((GmoNode *)node)->localMatrix;
    }
    node = GfxModelFindNode(&self->base.base, "context01");
    if (node != NULL) {
        self->nodeMatrices[2] = (ScePspFMatrix4 *)((GmoNode *)node)->localMatrix;
    }
    self->attackRange = 5000.0f;
    self->base.base.ambient[0] = 0.5f;
    self->base.base.ambient[1] = 0.5f;
    self->base.base.ambient[2] = 0.5f;
    self->base.base.ambient[3] = 1.0f;
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;
    GfxModelSetSpecular(10000.0f, &self->base.base, color, NULL);
    self->base.hpThreshold = 0.0f;
    if (BtlIsScoreMode(1) != 0) {
        u32 defType;

        self->autoFire = 1;
        defType = ActorCrystalDefaultType(self);
        ActorCrystalSetStyle(self, (s32)defType, (s32)ActorCrystalRandomStyle(self));
    } else {
        ActorCrystalSetStyle(self, type, style);
    }
    self->focusCamera = 1;
    for (i = 0; i < 3; i++) {
        /* random in [0, 1): vrndf1 minus bank S733 (1) */
        jitter = PlatformRandFloat12() - 1.0f;
        angle = ((float)i * 0.33f + 0.15f + jitter * 0.23f) * 6.2831855f;
        /* vrot [C,0,S,0] of angle * S703 (2/pi), scaled by 700, plus the model position */
        px = __builtin_cosf(angle) * 700.0f;
        pz = __builtin_sinf(angle) * 700.0f;
        self->orbitPoints[i][0] = px + self->base.base.pos[0];
        self->orbitPoints[i][1] = 0.0f + self->base.base.pos[1];
        self->orbitPoints[i][2] = pz + self->base.base.pos[2];
        self->orbitPoints[i][3] = 0.0f;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    boxMem = MemAlloc(0xc0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    box = NULL;
    if (boxMem != NULL) {
        boxMem->vtbl = g_collisionBoxVtbl;
        boxMem->invValid = 0;
        boxMem->type = 6;
        box = boxMem;
    }
    self->collisionBox = box;
    data = self->base.base.data;
    /* model bbox min/max -> box extents */
    src = data->bbox;
    box->aabbMin.x = src[0];
    box->aabbMin.y = src[1];
    box->aabbMin.z = src[2];
    box->aabbMin.w = src[3];
    box->aabbMax.x = src[4];
    box->aabbMax.y = src[5];
    box->aabbMax.z = src[6];
    box->aabbMax.w = src[7];
    box->invValid = 0;
    box->transform = *(ScePspFMatrix4 *)data->rootMatrix;
    if (!self->collisionBox->invValid) {
        CrystalBoxBuildInverse(self->collisionBox);
        self->collisionBox->invValid = 1;
    }
    k = self->base.base.scale[0] * 0.9f;
    self->collisionBox->aabbMin.x = self->collisionBox->aabbMin.x * k;
    self->collisionBox->aabbMin.z = self->collisionBox->aabbMin.z * k;
    self->collisionBox->aabbMax.x = self->collisionBox->aabbMax.x * k;
    self->collisionBox->aabbMax.z = self->collisionBox->aabbMax.z * k;
    self->collisionBox->aabbMax.y = self->collisionBox->aabbMax.y - 100.0f;
    if (self->collisionBox->aabbMax.y <= 30.0f) {
        self->collisionBox->aabbMax.y = 30.0f;
    }
    self->attackParam1 = 40;
    self->attackParam0 = 40;
    ActorCrystalUpdateMatrices(self);
    return self;
}
