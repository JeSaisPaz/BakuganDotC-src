// bdc 0x08900188 BtlDemoStateLoad
#include "bdc.h"

/* State 0 update of the battle intro demo task (task id 0x65, `BtlDemoCtor`, update
   `BtlDemoUpdate` / draw `BtlDemoDraw` on the state `+0x65c`): a step machine on `step` that
   preloads the demo's sound stream (`BtlDemoGetStreamId` + `SndStreamFilePreload`, waiting on
   `SndStreamFileIsLoaded`), starts the arena photo task 0x6c when two units share a species
   (`BtlHasDuplicateSpecies`, waiting on `g_btlArenaPhotoDone`), loads the demo's
   `data/event/<scb>.lzs` package unless its `.scb` is already present, builds the arena into
   `mapModels` (`BtlStageLoadMap` arena 0) and sets `g_btlDemoFinished`. Step 11 waits for
   `g_btlDemoReady`, then pauses task 100 (the battle), creates the two particle sets
   (`"particle_00.ptb"`, `"particle_01.ptb"`, `GfxEffectMgrCtor`), records each unit's
   position/heading in `placements`, places the player's Bakugan at the stage origin
   (`BtlGetStageAmbientColor`, heading pi/2), aims the demo camera at it with per-demo clip/fov
   tweaks, creates the scene players (plus one or two `ActorBall`s for variant-1 demos), loads
   `"demo_start.gmo"`, hides the HUD tasks 0x1e0/0x1e1, starts the stream on BGM player 1 and
   switches to state 1 (step 0, flash alpha 0.3). Returns early (state unchanged) whenever a step
   is still waiting.
   VFPU: the velocity vectors of the Bakugan, the balls and the stage model are stored from the
   bank constant C720 (= 0, 0, 0, 0); the 16-byte lv.q/sv.q copies are plain vector copies. */

/* 16-byte vector copy `dst = src` (lv.q/sv.q through C000; nobody reads C000 afterwards). */
static inline void BtlDemoQuadCopy(float *dst, const float *src)
{
    float x = src[0], y = src[1], z = src[2], w = src[3];

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
    dst[3] = w;
}

/* Stores the bank constant C720 (the zero vector) to `dst`. */
static inline void BtlDemoStoreC720(float *dst)
{
    dst[0] = 0.0f;
    dst[1] = 0.0f;
    dst[2] = 0.0f;
    dst[3] = 0.0f;
}

/* Places `model` at `origin` (heading `origin[3]`) the way the demo places its balls. */
static inline void BtlDemoPlaceBall(GfxModel *model, float *origin)
{
    ActorBallSetHeading(origin[3], model);
    BtlDemoQuadCopy(model->pos, origin);
    BtlDemoStoreC720(model->velocity);
    BtlDemoQuadCopy(&model->data->rootMatrix[12], origin);
    ActorBallSetHeading(origin[3], model);
}

void BtlDemoStateLoad(BtlDemo *demo)
{
    char name[128];
    float color[4] __attribute__((aligned(16)));
    float scale[4] __attribute__((aligned(16)));
    float origin[4] __attribute__((aligned(16)));
    float origin2[4] __attribute__((aligned(16)));
    BtlDemoCam *cam;
    BtlMain *battle;
    CoreTask *task;
    CoreObjectList *list;
    CoreObject *obj;
    GfxEffectMgr *mgr;
    BtlBakugan *bakugan;
    ActorBall *ball;
    ActorBall *ball2;
    GfxModel *model;
    SndBgmPlayer *player;
    const VtblEntry *entry;
    char *dot;
    bool low;
    s32 id;
    s32 i;

    switch (demo->step) {
    case 0:
        demo->streamId = BtlDemoGetStreamId(demo->demoId);
        if (demo->streamId != -1) {
            SndStreamFilePreload(demo->streamId);
            demo->step++;
            return;
        }
        demo->step = 2;
        /* fall through */
    case 1:
        if (demo->streamId != -1) {
            if (!SndStreamFileIsLoaded(demo->streamId)) {
                return;
            }
            demo->step++;
        }
        /* fall through */
    case 2:
        if (!BtlHasDuplicateSpecies()) {
            demo->step = 8;
            return;
        }
        CoreTaskCreate(0x6c, 100);
        demo->step++;
        /* fall through */
    case 3:
        if (g_btlArenaPhotoDone != 1) {
            return;
        }
        demo->step = 8;
        /* fall through */
    case 8:
        if (BtlDemoScbExists(demo->demoId)) {
            demo->step = 10;
        } else {
            strcpy(name, BtlDemoScbGetName(demo->demoId));
            dot = strrchr(name, '.');
            if (dot != NULL) {
                *dot = '\0';
            }
            strcat(name, ".lzs");
            strcpy(demo->packagePath, "data/event/");
            strcat(demo->packagePath, name);
            if (IoLzsPackageStartLoad(&demo->packages[0], demo->packagePath, 10, 1, 0) != 0) {
                demo->step++;
            }
        }
        /* fall through */
    case 9:
        if (IoLzsPackagePoll(&demo->packages[0], 1) == 0) {
            return;
        }
        demo->step = 10;
        /* fall through */
    case 10:
        if (demo->mapModels == NULL) {
            MemLock();
            low = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            list = MemAlloc(sizeof(CoreObjectList), NULL, 0);
            MemSetAllocFromLow(low);
            MemUnlock();
            demo->mapModels = list;
            list->tail = NULL;
            list->head = NULL;
            list->count = 0;
            BtlStageLoadMap(0, demo->mapModels);
            GfxPuffLoadTexture();
            GfxMeshObjClearLists();
            ActorStageObjSystemInit();
        }
        BtlSetControlLockAll(1);
        g_btlDemoFinished = 1;
        demo->step++;
        return;
    case 11:
        break;
    default:
        return;
    }

    /* step 11: wait for the battle, then build the demo scene */
    if (g_btlDemoReady == 0) {
        return;
    }
    battle = (BtlMain *)CoreTaskFind(100);
    cam = &demo->cam;
    if (battle != NULL) {
        CoreTaskSetFlags(&battle->base, 1);
    }
    BtlItemListDespawnAll();
    BtlSetFieldEffectsActive(false);

    MemLock();
    low = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
    MemSetAllocFromLow(low);
    MemUnlock();
    if (mgr != NULL) {
        GfxEffectMgrCtor(mgr, CorePackChainFind(g_ioLzsPackages, "particle_00.ptb"));
    }
    demo->particles0 = mgr;

    MemLock();
    low = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
    MemSetAllocFromLow(low);
    MemUnlock();
    if (mgr != NULL) {
        GfxEffectMgrCtor(mgr, CorePackChainFind(g_ioLzsPackages, "particle_01.ptb"));
    }
    demo->particles1 = mgr;

    if (battle != NULL) {
        demo->spriteLayer = battle->spriteLayers[1];
        demo->effectLayer = battle->spriteLayers[2];
    }
    g_worldEffectMgr = demo->particles0;
    g_btlUnitEffectMgr = demo->particles1;
    GfxSetMotionTimeScale(1.0f);

    BtlGetStageAmbientColor(color);
    BtlDemoQuadCopy(origin, color);
    origin[3] = 1.5707964f;
    SaveGetTeamBakugan(-1);

    /* record each unit's position and heading (units whose entry 14 returns 0) */
    list = (CoreObjectList *)BtlGetBakuganList();
    if (list != NULL) {
        i = 0;
        for (obj = list->head; obj != NULL; obj = obj->next) {
            entry = &((const VtblEntry *)obj->vtable)[14];
            if (((int (*)(void *))entry->fn)((u8 *)obj + entry->delta) != 0) {
                continue;
            }
            bakugan = (BtlBakugan *)obj;
            BtlDemoQuadCopy(demo->placements[i], bakugan->base.pos);
            demo->placements[i][3] = bakugan->base.rot[1];
            if (bakugan->collider0 != NULL) {
                bakugan->collider0->hitTimer = 0;
                bakugan->collider0->flags |= 1;
            }
            i++;
        }
    }

    /* the player's Bakugan goes to the stage origin */
    bakugan = BtlGetPlayerBakugan();
    if (bakugan->collider1 != NULL) {
        bakugan->collider1->flags |= 0x46;
    }
    BtlBakuganSetHeading(bakugan, origin[3]);
    BtlDemoQuadCopy(bakugan->base.pos, origin);
    bakugan->isPlayer = 1;
    demo->bakugan = bakugan;
    BtlDemoLoadMotions(demo);

    if (BtlCameraTaskExists()) {
        demo->battleCamera = &((BtlMain *)BtlGetCameraTask())->camera.base;
    }
    BtlDemoCamSetTarget(cam, demo->bakugan);
    BtlDemoCamResetKeysFar(cam);
    demo->cam.shotId = demo->demoId;
    BtlDemoCamUpdate(cam);
    g_gfxActiveCamera = &cam->base;
    demo->cam.base.nearZ = 30.0f;
    GfxWaitGeIdle();
    task = CoreTaskFind(100);
    if (task != NULL) {
        CoreTaskSetFlags(task, 2);
    }
    CoreTaskClearFlags(&demo->base, 2);

    scale[2] = 1.0f;
    scale[1] = 1.0f;
    scale[0] = 1.0f;
    scale[3] = 0.0f;
    BtlDemoQuadCopy(demo->bakugan->base.scale, scale);

    switch (demo->demoId) {
    case 0x30:
    case 0x32:
        demo->cam.base.nearZ = 20.0f;
        break;
    case 0x38:
        demo->cam.base.nearZ = 100.0f;
        break;
    case 0x42:
        demo->cam.fov = 50.0f;
        demo->cam.base.frustumScale = 1.1f;
        demo->cam.base.nearZ = 20.0f;
        break;
    case 0x46:
        demo->cam.base.nearZ = 24.0f;
        break;
    case 0x50:
    case 0x52:
    case 0x66:
        demo->cam.fov = 50.0f;
        demo->cam.base.frustumScale = 1.1f;
        break;
    default:
        break;
    }
    BtlDemoCamUpdate(cam);

    BtlDemoStoreC720(demo->bakugan->base.velocity);
    BtlDemoQuadCopy(&demo->bakugan->base.data->rootMatrix[12], origin);
    BtlBakuganSetHeading(demo->bakugan, origin[3]);
    bakugan = demo->bakugan;
    BtlDemoQuadCopy(bakugan->base.pos, &bakugan->base.data->rootMatrix[12]);
    BtlBakuganSetState(demo->bakugan, 0xf, 0);
    demo->bakugan->base.ambient[3] = 1.0f;
    bakugan = demo->bakugan;
    entry = &((const VtblEntry *)bakugan->base.base.vtable)[6];
    ((void (*)(float, void *))entry->fn)(1.0f, (u8 *)bakugan + entry->delta);
    demo->bakugan->groundPoint[1] = 0.0f;
    if (demo->demoId == 0x3a) {
        BtlBakuganSetHeading(demo->bakugan, origin[3] - 0.3926991f);
    }
    demo->scenePlayers[0] = BtlDemoScenePlayerCreate(demo->demoId, demo->bakugan, NULL, NULL);

    if (BtlDemoIdIsVariant1(demo->demoId)) {
        /* the team Bakugan's ball */
        ball = (ActorBall *)ActorBallCreate(0.1f, SaveGetTeamBakuganModelId(-1), 0, NULL);
        BtlDemoPlaceBall(&ball->base, origin);
        ball->noGroundProbe = 1;
        ball->groundPoint[1] = origin[1];
        entry = &((const VtblEntry *)ball->base.base.vtable)[7];
        ((void (*)(void *))entry->fn)((u8 *)ball + entry->delta);
        ActorBallMaybeApplyTextureVariant(ball,
            BtlBakuganGetSameKindIndex(BtlFindTeamBakugan((u32)-1)));
        demo->balls[0] = &ball->base.base;
        demo->scenePlayers[1] = BtlDemoScenePlayerCreate(0x16, NULL, NULL, ball);

        /* Bakugan 2 and 10 get a second ball */
        id = SaveGetTeamBakugan(-1);
        if (id == 2 || id == 10) {
            BtlDemoQuadCopy(origin2, origin);
            ball2 = (ActorBall *)ActorBallCreate(0.1f, id + 0x57, 0, NULL);
            BtlDemoPlaceBall(&ball2->base, origin2);
            ActorBallMaybeApplyTextureVariant(ball2,
                BtlBakuganGetSameKindIndex(BtlFindTeamBakugan((u32)-1)));
            entry = &((const VtblEntry *)ball2->base.base.vtable)[7];
            ((void (*)(void *))entry->fn)((u8 *)ball2 + entry->delta);
            ball2->base.ambient[3] = 0.0f;
            demo->balls[1] = &ball2->base.base;
            demo->scenePlayers[2] =
                BtlDemoScenePlayerCreate(id == 10 ? 0x68 : 0x67, NULL, NULL, demo->balls[1]);
        }
    }

    if (CorePackChainFindData(g_ioLzsPackages, "demo_start.gmo") != NULL) {
        MemLock();
        low = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        model = MemAlloc(sizeof(GfxModel), NULL, 0);
        MemSetAllocFromLow(low);
        MemUnlock();
        if (model != NULL) {
            GfxModelCtor(model, "demo_start.gmo", 0);
        }
        demo->stageModel = model;
        BtlDemoQuadCopy(model->pos, origin);
        BtlDemoStoreC720(demo->stageModel->velocity);
        BtlDemoQuadCopy(&demo->stageModel->data->rootMatrix[12], origin);
        demo->stageModel->lighting = 1;
        BtlStageApplyLightColors(demo->stageModel);
        GmoModelSetFlags28(demo->stageModel->data, 0x100, 0xffffffff);
        demo->stageModel->ambient[3] = 0.0f;
        CoreObjectListAppend(&demo->stageModel->base, demo->mapModels);
    }

    BtlDemoScenePlayerPrime(&demo->base, demo->scenePlayers[0]);
    if (demo->scenePlayers[1] != NULL) {
        demo->scenePlayers[0]->suppressDemoEnd = 1;
        CoreTaskSetFlags(&demo->scenePlayers[1]->base, 3);
    }
    if (demo->scenePlayers[2] != NULL) {
        CoreTaskSetFlags(&demo->scenePlayers[2]->base, 3);
        demo->scenePlayers[2]->suppressDemoEnd = 1;
    }
    BtlDemoCamUpdate(cam);

    /* result window of a finished battle: outcome 1 -> 4, 2..3 -> 5, 4 -> 6 */
    if (g_btlBattleOver != 0) {
        if (g_btlBattleOutcome < 2) {
            if (g_btlBattleOutcome > 0) {
                UiSetWindowActive(4, 1);
            }
        } else if (g_btlBattleOutcome < 4) {
            UiSetWindowActive(5, 1);
        } else if (g_btlBattleOutcome < 5) {
            UiSetWindowActive(6, 1);
        }
    }
    if (CoreTaskExists(0x1e0)) {
        CoreTaskSetFlags(CoreTaskFind(0x1e0), 2);
    }
    if (CoreTaskExists(0x1e1)) {
        CoreTaskSetFlags(CoreTaskFind(0x1e1), 2);
    }
    if (demo->bakugan != NULL) {
        BtlBakuganStopEffectsAndAttacks(demo->bakugan);
    }
    BtlDemoCamBindListener(cam);
    BtlDemoUpdateObjects(demo);
    if (demo->streamId != -1 && SndBgmPlayerExists(1)) {
        player = SndBgmPlayerGet(1);
        SndBgmPlayerPlayTrack(player, demo->streamId, 0, 0);
    }
    demo->state = 1;
    demo->step = 0;
    demo->flashAlpha = 0.3f;
}
