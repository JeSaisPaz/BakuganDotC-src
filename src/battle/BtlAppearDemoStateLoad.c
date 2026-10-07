// bdc 0x08901e74 BtlAppearDemoStateLoad
#include "bdc.h"

/* State 0 (loading) of the Bakugan-appearance demo task (task id 0x67, `BtlAppearDemoCtor`,
   update `BtlAppearDemoUpdate`), one `step` per call:
   0/1 preload the demo's sound stream (`BtlDemoGetStreamId`, skipped when it has none) and wait
   for it; 2/3 in script mode 1 (`ScriptVarsModeIs1`) load and wait for
   `"data/event/12_baku_app_%02d.lzs"` (team Bakugan); 4/5 when the battle has duplicate species and
   the team Bakugan is 2 or 10, load and wait for its ball texture package
   `"data/battle/sphere/<name>_tex.lzs"` (else step 6); 6/7 lock control, load and wait for the
   scene package `"data/event/<scb name>.lzs"`; 8 builds the arena map once (map model list,
   `BtlStageLoadMap` arena 0, smoke texture, mesh lists, stage objects) and sets
   `g_btlDemoFinished`; 9 hides the battle main task (id 100), turns the field effects off,
   creates the particle sets `particle_00/01.ptb` (published as `g_worldEffectMgr` /
   `g_btlUnitEffectMgr`), borrows sprite layers 1/2 of the battle main task, spawns the brawler
   actor (`SaveGetTeamBakuganFace`) at the stage point (the per-battle vector of
   `BtlGetStageAmbientColor`, heading pi/2) in state 3 / motion 8, the team Bakugan's ball 5
   units to the side (a second ball for Bakugan 2/10), the `"demo_start.gmo"` model when there is
   no second ball and the package holds it, records position and heading of every unit whose
   vtable entry 14 returns 0 in `placements`, puts the player Bakugan at the stage point in state
   0xf, loads the demo motions, sets demo-specific motion speeds (vtable entry 6), aims the demo
   camera at the actor and creates the three scene players (`BtlDemoScenePlayerCreate`); 10
   starts the stream on BGM player 0 and waits 8 frames (11); then (and for any other step) stops
   the Bakugan's effects, binds the listener, updates the objects once and switches to state 1
   with step 0 and flash alpha 0.3. Returns nothing.
   The velocity reset of the actor, balls, stage model and Bakugan stores the bank constant C720
   (all zero). */

/* Quad copy (lv.q/sv.q through C000 in the listing). */
static inline void BtlAppearDemoCopyQuad(float *dst, const float *src)
{
    float x = src[0];
    float y = src[1];
    float z = src[2];
    float w = src[3];

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
    dst[3] = w;
}

/* Stores the bank constant C720 = (0, 0, 0, 0) into `dst`. */
static inline void BtlAppearDemoZeroQuad(float *dst)
{
    dst[0] = 0.0f;
    dst[1] = 0.0f;
    dst[2] = 0.0f;
    dst[3] = 0.0f;
}

/* Calls vtable entry 7 (motion update) of a model object. */
static inline void BtlAppearDemoCallEntry7(CoreObject *obj)
{
    const VtblEntry *entry;

    entry = &((const VtblEntry *)obj->vtable)[7];
    ((void (*)(void *))entry->fn)((u8 *)obj + entry->delta);
}

/* Calls vtable entry 6 (float setter, here the motion speed) of a model object. */
static inline void BtlAppearDemoCallEntry6(CoreObject *obj, float value)
{
    const VtblEntry *entry;

    entry = &((const VtblEntry *)obj->vtable)[6];
    ((void (*)(void *, float))entry->fn)((u8 *)obj + entry->delta, value);
}

/* Places a freshly created ball at `pos` (heading pos[3]) and applies the team Bakugan's
   duplicate-species texture variant, then runs its motion update. */
static inline void BtlAppearDemoPlaceBall(ActorBall *ball, float *pos)
{
    ActorBallSetHeading(pos[3], &ball->base);
    BtlAppearDemoCopyQuad(ball->base.pos, pos);
    BtlAppearDemoZeroQuad(ball->base.velocity);
    BtlAppearDemoCopyQuad(&ball->base.data->rootMatrix[12], pos);
    ActorBallSetHeading(pos[3], &ball->base);
}

void BtlAppearDemoStateLoad(BtlAppearDemo *demo)
{
    char texName[32];
    char sceneName[128];
    float color0[4];
    float stagePos[4];
    float ballPos[4];
    float color1[4];
    float scale[4];
    float bakuganPos[4];
    BtlMain *btlMain;
    GfxEffectMgr *mgr;
    GfxEffectMgr *mgrMem;
    CoreObjectList *list;
    GfxModel *model;
    GfxModel *modelMem;
    Actor *actor;
    ActorBall *ball;
    BtlBakugan *bakugan;
    BtlBakugan *unit;
    CoreObjectList *units;
    const VtblEntry *entry;
    float (*placement)[4];
    const char *gmoName;
    char *path;
    char *dot;
    bool fromLow;
    s32 id;
    s32 kind;
    s32 sceneId1;
    s32 sceneId2;

    switch (demo->base.step) {
    case 0:
        id = BtlDemoGetStreamId(demo->base.demoId);
        demo->base.streamId = id;
        if (id != -1) {
            SndStreamFilePreload(demo->base.streamId);
            demo->base.step++;
            return;
        }
        demo->base.step = 2;
        /* fallthrough */
    case 1:
        if (demo->base.streamId != -1) {
            if (!SndStreamFileIsLoaded(demo->base.streamId)) {
                return;
            }
            demo->base.step++;
        }
        /* fallthrough */
    case 2:
        if (ScriptVarsModeIs1()) {
            id = SaveGetTeamBakugan(-1);
            path = &demo->base.packagePath[0x40];
            sprintf(path, "data/event/12_baku_app_%02d.lzs", id);
            if (IoLzsPackageStartLoad(&demo->base.packages[1], path, 10, 1, 0) == 0) {
                return;
            }
        }
        demo->base.step++;
        /* fallthrough */
    case 3:
        if (ScriptVarsModeIs1()) {
            if (IoLzsPackagePoll(&demo->base.packages[1], 1) == 0) {
                return;
            }
        }
        demo->base.step++;
        /* fallthrough */
    case 4:
        if (!BtlHasDuplicateSpecies()) {
            demo->base.step = 6;
            return;
        }
        kind = SaveGetTeamBakugan(-1);
        if (kind != 2 && kind != 10) {
            demo->base.step = 6;
            return;
        }
        strcpy(texName, g_btlUnitKindNames[kind + 0x57]);
        strcat(texName, "_tex");
        path = &demo->base.packagePath[0x80];
        sprintf(path, "data/battle/sphere/%s.lzs", texName);
        if (IoLzsPackageStartLoad(&demo->base.packages[2], path, 10, 1, 0) != 0) {
            demo->base.step++;
        }
        return;

    case 5:
        if (IoLzsPackagePoll(&demo->base.packages[2], 1) == 0) {
            return;
        }
        demo->base.step++;
        /* fallthrough */
    case 6:
        BtlSetControlLockAll(1);
        strcpy(sceneName, BtlDemoScbGetName(demo->base.demoId));
        dot = strrchr(sceneName, '.');
        if (dot != NULL) {
            *dot = '\0';
        }
        strcat(sceneName, ".lzs");
        strcpy(demo->base.packagePath, "data/event/");
        strcat(demo->base.packagePath, sceneName);
        if (IoLzsPackageStartLoad(&demo->base.packages[0], demo->base.packagePath, 10, 1, 0) != 0) {
            demo->base.step++;
        }
        /* fallthrough */
    case 7:
        if (IoLzsPackagePoll(&demo->base.packages[0], 1) == 0) {
            return;
        }
        demo->base.step++;
        /* fallthrough */
    case 8:
        if (demo->base.mapModels == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            list = MemAlloc(sizeof(CoreObjectList), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            demo->base.mapModels = list;
            list->tail = NULL;
            list->head = NULL;
            list->count = 0;
            BtlStageLoadMap(0, demo->base.mapModels);
            GfxPuffLoadTexture();
            GfxMeshObjClearLists();
            ActorStageObjSystemInit();
        }
        g_btlDemoFinished = 1;
        demo->base.step++;
        return;

    case 9:
        btlMain = CoreTaskFind(100);
        if (btlMain != NULL) {
            CoreTaskSetFlags(&btlMain->base, 3);
        }
        BtlSetFieldEffectsActive(false);

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mgrMem = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        mgr = NULL;
        if (mgrMem != NULL) {
            GfxEffectMgrCtor(mgrMem, CorePackChainFind(g_ioLzsPackages, "particle_00.ptb"));
            mgr = mgrMem;
        }
        demo->base.particles0 = mgr;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mgrMem = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        mgr = NULL;
        if (mgrMem != NULL) {
            GfxEffectMgrCtor(mgrMem, CorePackChainFind(g_ioLzsPackages, "particle_01.ptb"));
            mgr = mgrMem;
        }
        demo->base.particles1 = mgr;

        if (btlMain != NULL) {
            demo->base.spriteLayer = btlMain->spriteLayers[1];
            demo->base.effectLayer = btlMain->spriteLayers[2];
        }
        g_worldEffectMgr = demo->base.particles0;
        g_btlUnitEffectMgr = demo->base.particles1;
        ActorSetList(&demo->base.actorList);

        /* the brawler actor at the stage point, facing pi/2 */
        BtlGetStageAmbientColor(color0);
        BtlAppearDemoCopyQuad(stagePos, color0);
        stagePos[3] = 1.57079637f;
        actor = ActorSpawn(SaveGetTeamBakuganFace(-1), 0, NULL);
        if (actor->bodyCollider != NULL) {
            ((CollisionCollider *)actor->bodyCollider)->flags |= 0xc7;
        }
        if (actor->collider2 != NULL) {
            ((CollisionCollider *)actor->collider2)->flags |= 0xc7;
        }
        ActorSetHeading(actor, stagePos[3]);
        BtlAppearDemoCopyQuad(actor->base.pos, stagePos);
        actor->isPlayer = 1;
        actor->fallSpeed = 0.0f;
        actor->jumpTimer = 999;
        BtlAppearDemoZeroQuad(actor->base.velocity);
        BtlAppearDemoCopyQuad(&actor->base.data->rootMatrix[12], stagePos);
        ActorSetHeading(actor, stagePos[3]);
        BtlAppearDemoCopyQuad(actor->base.pos, &actor->base.data->rootMatrix[12]);
        ActorSetStateBase(actor, 3, 0);
        ActorPlayMotion(0.0f, actor, 8, 0, 1);
        BtlAppearDemoCallEntry7(&actor->base.base);
        demo->base.actor = actor;
        if (ScriptVarsModeIs1()) {
            ActorEditManLoadAppearMotions(demo->base.actor);
        }

        /* the team Bakugan's ball, 5 units to the side */
        id = SaveGetTeamBakuganModelId(-1);
        BtlAppearDemoCopyQuad(ballPos, stagePos);
        ballPos[0] = ballPos[0] + 5.0f;
        ball = (ActorBall *)ActorBallCreate(0.1f, id, 0, NULL);
        BtlAppearDemoPlaceBall(ball, ballPos);
        ball->noGroundProbe = 1;
        ball->groundPoint[1] = stagePos[1];
        ActorBallMaybeApplyTextureVariant(ball,
            BtlBakuganGetSameKindIndex(BtlFindTeamBakugan((u32)-1)));
        BtlAppearDemoCallEntry7(&ball->base.base);
        demo->base.balls[0] = &ball->base.base;

        kind = SaveGetTeamBakugan(-1);
        if (kind == 2 || kind == 10) {
            ball = (ActorBall *)ActorBallCreate(0.1f, kind + 0x57, 0, NULL);
            BtlAppearDemoPlaceBall(ball, ballPos);
            ActorBallMaybeApplyTextureVariant(ball,
                BtlBakuganGetSameKindIndex(BtlFindTeamBakugan((u32)-1)));
            BtlAppearDemoCallEntry7(&ball->base.base);
            ball->base.ambient[3] = 0.0f;
            demo->base.balls[1] = &ball->base.base;
        }

        if (demo->base.balls[1] == NULL) {
            gmoName = "demo_start.gmo";
            if (CorePackChainFindData(g_ioLzsPackages, (char *)gmoName) != NULL) {
                MemLock();
                fromLow = MemIsAllocFromLow();
                MemSetAllocFromLow(true);
                modelMem = MemAlloc(sizeof(GfxModel), NULL, 0);
                MemSetAllocFromLow(fromLow);
                MemUnlock();
                model = NULL;
                if (modelMem != NULL) {
                    GfxModelCtor(modelMem, gmoName, 0);
                    model = modelMem;
                }
                demo->base.stageModel = model;
                BtlAppearDemoCopyQuad(model->pos, stagePos);
                BtlAppearDemoZeroQuad(demo->base.stageModel->velocity);
                BtlAppearDemoCopyQuad(&demo->base.stageModel->data->rootMatrix[12], stagePos);
                demo->base.stageModel->lighting = 1;
                BtlStageApplyLightColors(demo->base.stageModel);
                demo->base.stageModel->ambient[3] = 0.0f;
                CoreObjectListAppend(&demo->base.stageModel->base, demo->base.mapModels);
                GmoModelSetFlags28(demo->base.stageModel->data, 0x100, 0xffffffff);
            }
        }

        /* the player Bakugan at the stage point; the other units' positions go to placements */
        BtlGetStageAmbientColor(color1);
        BtlAppearDemoCopyQuad(bakuganPos, color1);
        bakuganPos[3] = 1.57079637f;
        SaveGetTeamBakugan(-1);
        bakugan = BtlGetPlayerBakugan();
        units = BtlGetBakuganList();
        if (units != NULL) {
            placement = demo->base.placements;
            for (unit = (BtlBakugan *)units->head; unit != NULL;
                 unit = (BtlBakugan *)unit->base.base.next) {
                entry = &((const VtblEntry *)unit->base.base.vtable)[14];
                if (((s32 (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
                    continue;
                }
                BtlAppearDemoCopyQuad(*placement, unit->base.pos);
                (*placement)[3] = unit->base.rot[1];
                if (unit->collider0 != NULL) {
                    unit->collider0->hitTimer = 0;
                    unit->collider0->flags |= 1;
                }
                placement++;
            }
        }
        BtlBakuganSetHeading(bakugan, bakuganPos[3]);
        BtlAppearDemoCopyQuad(bakugan->base.pos, bakuganPos);
        bakugan->isPlayer = 1;
        BtlAppearDemoZeroQuad(bakugan->base.velocity);
        BtlAppearDemoCopyQuad(&bakugan->base.data->rootMatrix[12], bakuganPos);
        BtlBakuganSetHeading(bakugan, bakuganPos[3]);
        BtlAppearDemoCopyQuad(bakugan->base.pos, &bakugan->base.data->rootMatrix[12]);
        BtlBakuganSetState(bakugan, 0xf, 0);
        bakugan->base.ambient[3] = 0.0f;
        bakugan->groundPoint[1] = bakuganPos[1];
        bakugan->gravityHold = 999;
        scale[2] = 1.0f;
        scale[1] = 1.0f;
        scale[0] = 1.0f;
        scale[3] = 0.0f;
        BtlAppearDemoCopyQuad(bakugan->base.scale, scale);
        demo->base.bakugan = bakugan;
        BtlDemoLoadMotions(&demo->base);

        switch (demo->base.demoId) {
        case 6:
        case 0x12:
            if (demo->base.actor != NULL) {
                BtlAppearDemoCallEntry6(&demo->base.actor->base.base, 1.04999995f);
            }
            if (demo->base.balls[0] != NULL) {
                BtlAppearDemoCallEntry6(demo->base.balls[0], 0.800000012f);
            }
            break;
        case 0xf:
        case 0x15:
            if (demo->base.actor != NULL) {
                BtlAppearDemoCallEntry6(&demo->base.actor->base.base, 1.10000002f);
            }
            break;
        }

        if (BtlCameraTaskExists()) {
            demo->base.battleCamera = &((BtlMain *)BtlGetCameraTask())->camera.base;
        }
        demo->base.cam.shotId = demo->base.demoId;
        BtlDemoCamSetTarget(&demo->base.cam, demo->base.actor);
        BtlDemoCamResetKeysFarB(&demo->base.cam);
        g_gfxActiveCamera = &demo->base.cam.base;

        sceneId1 = BtlDemoIdVariant2(demo->base.bakugan != NULL ? (s32)demo->base.bakugan->base.base.unk08 : 1);
        sceneId2 = BtlDemoIdVariant0(demo->base.bakugan != NULL ? (s32)demo->base.bakugan->base.base.unk08 : 1);
        if (demo->base.balls[1] != NULL) {
            demo->scenePlayers[1] = BtlDemoScenePlayerCreate(sceneId1, NULL, NULL, demo->base.balls[1]);
        } else {
            demo->scenePlayers[1] = BtlDemoScenePlayerCreate(sceneId1, NULL, NULL, demo->base.balls[0]);
        }
        demo->scenePlayers[2] = BtlDemoScenePlayerCreate(sceneId2, demo->base.bakugan, demo->base.actor, NULL);
        demo->scenePlayers[0] = BtlDemoScenePlayerCreate(demo->base.demoId, NULL, demo->base.actor,
                                                         demo->base.balls[0]);
        BtlDemoScenePlayerPrime(&demo->base.base, demo->scenePlayers[0]);
        CoreTaskSetFlags(&demo->scenePlayers[1]->base, 3);
        CoreTaskSetFlags(&demo->scenePlayers[2]->base, 3);
        demo->scenePlayers[0]->suppressDemoEnd = 1;
        demo->scenePlayers[1]->suppressDemoEnd = 1;
        BtlDemoCamUpdate(&demo->base.cam);
        BtlDemoCamUpdate(&demo->base.cam);
        demo->base.step++;
        /* fallthrough */
    case 10:
        if (demo->base.streamId != -1 && SndBgmPlayerExists(0)) {
            SndBgmPlayerPlayTrack(SndBgmPlayerGet(0), demo->base.streamId, 0, 0);
        }
        demo->waitFrames = 8;
        demo->base.step++;
        /* fallthrough */
    case 11:
        if (demo->waitFrames-- > 0) {
            return;
        }
        demo->base.step++;
        /* fallthrough */
    default:
        if (demo->base.bakugan != NULL) {
            BtlBakuganStopEffectsAndAttacks(demo->base.bakugan);
        }
        BtlDemoCamBindListener(&demo->base.cam);
        BtlDemoUpdateObjects(&demo->base);
        demo->base.state = 1;
        demo->base.step = 0;
        demo->base.flashAlpha = 0.300000012f;
        return;
    }
}
