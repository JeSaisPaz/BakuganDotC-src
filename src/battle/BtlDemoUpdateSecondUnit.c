// bdc 0x08900db4 BtlDemoUpdateSecondUnit
#include "bdc.h"

/* Step machine on `secondStep` of the battle intro demo task (task id 0x65, `BtlDemoCtor`), run
   every frame by `BtlDemoStatePlay`; returns at once while `scenePlayers[1]` is NULL and for
   steps 5..9 and >= 12. Step 0 advances to 1; step 1 waits for `scenePlayers[0]->finished`; step 2
   (entered with it) sets the camera shot `cam.shotId` to 0x68 when the active team Bakugan is id 10
   (`SaveGetTeamBakugan(-1)`), else 0x67, and goes to 3 — or straight to 10 when `scenePlayers[2]`
   is NULL. Step 3 poses `balls[1]` and starts `scenePlayers[2]`: position = the stage vector of
   `BtlGetStageAmbientColor` with w = pi/2, heading pi/2 (`ActorBallSetHeading`), velocity =
   zero (bank C720), the GMO root translation = the same vector; camera keys reset
   (`BtlDemoCamResetKeysNear`) with fov 70; the player is unhidden (`CoreTaskClearFlags` 3),
   primed (`BtlDemoScenePlayerPrime`) and run 3 updates (vtable entry 2) while `scenePlayers[0]`
   is hidden/frozen (`CoreTaskSetFlags` 3); the demo Bakugan gets alpha 0 and the same position
   and one motion update (vtable entry 7); flash off with alpha 0.3; the ball gets one motion
   update, the camera targets it (`BtlDemoCamSetTarget`) and updates twice (`BtlDemoCamUpdate`);
   effect 0x150 spawns on `g_btlUnitEffectMgr` at the ball's node 0 (team Bakugan id 2), node 0x45
   (id 10) or the origin (`GfxEffectSpawn`); the battle main task (task 100, `CoreTaskFind`)
   gets a scene update (`BtlMainUpdateScene`); then step 4. Step 4 waits for
   `scenePlayers[2]->finished`, restores `cam.shotId = demoId`, hides that player and enters
   step 10. Step 10 does the same pose for `balls[0]` with `scenePlayers[1]` (9 updates, no effect),
   then step 11; when there is a `stageModel` it gets alpha 1 and a motion update while arena model
   0 (`g_btlArenaModels`) gets alpha 0. Step 11 swings the camera: `swingAngle` += 0.035,
   clamped to [0, pi/2], fov = 70 - 25 * sin(swingAngle) (vsin of swingAngle times the bank's 2/pi). */

static void CopyVec4(float *dst, const float *src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

void BtlDemoUpdateSecondUnit(BtlDemo *demo)
{
    float stage[4] __attribute__((aligned(16)));
    float pose[4] __attribute__((aligned(16)));
    float spawnPos[4] __attribute__((aligned(16)));
    ScePspFVector4 nodePos __attribute__((aligned(16)));
    const VtblEntry *entry;
    BtlDemoCam *cam;
    BtlMain *main;
    GfxModel *ball;
    s32 team;
    s32 shot;
    int i;
    float swing;

    if (demo->scenePlayers[1] == NULL) {
        return;
    }
    switch ((u32)demo->secondStep) {
    case 0:
        demo->secondStep = demo->secondStep + 1;
        /* fallthrough */
    case 1:
        if (demo->scenePlayers[0]->finished == 0) {
            return;
        }
        demo->secondStep = demo->secondStep + 1;
        /* fallthrough */
    case 2:
        if (demo->scenePlayers[2] == NULL) {
            demo->secondStep = 10;
            return;
        }
        team = SaveGetTeamBakugan(-1);
        shot = 0x67;
        if (team == 10) {
            shot = 0x68;
        }
        demo->cam.shotId = shot;
        demo->secondStep = demo->secondStep + 1;
        return;

    case 3:
        BtlGetStageAmbientColor(stage);
        CopyVec4(pose, stage);
        pose[3] = 1.57079637f;
        ActorBallSetHeading(1.57079637f, (GfxModel *)demo->balls[1]);
        ball = (GfxModel *)demo->balls[1];
        CopyVec4(ball->pos, pose);
        ball = (GfxModel *)demo->balls[1];
        ball->velocity[0] = 0.0f;
        ball->velocity[1] = 0.0f;
        ball->velocity[2] = 0.0f;
        ball->velocity[3] = 0.0f;
        ball = (GfxModel *)demo->balls[1];
        CopyVec4(&ball->data->rootMatrix[12], pose);
        ActorBallSetHeading(pose[3], (GfxModel *)demo->balls[1]);
        cam = &demo->cam;
        BtlDemoCamResetKeysNear(cam);
        demo->cam.fov = 70.0f;
        CoreTaskClearFlags(&demo->scenePlayers[2]->base, 3);
        BtlDemoScenePlayerPrime(&demo->base, demo->scenePlayers[2]);
        for (i = 0; i < 3; i++) {
            entry = &((const VtblEntry *)demo->scenePlayers[2]->base.vtable)[2];
            ((void (*)(void *))entry->fn)((u8 *)demo->scenePlayers[2] + entry->delta);
        }
        CoreTaskSetFlags(&demo->scenePlayers[0]->base, 3);
        demo->bakugan->base.ambient[3] = 0.0f;
        CopyVec4(demo->bakugan->base.pos, pose);
        entry = &((const VtblEntry *)demo->bakugan->base.base.vtable)[7];
        ((void (*)(void *))entry->fn)((u8 *)demo->bakugan + entry->delta);
        demo->flashOn = 0;
        demo->flashAlpha = 0.300000012f;
        entry = &((const VtblEntry *)demo->balls[1]->vtable)[7];
        ((void (*)(void *))entry->fn)((u8 *)demo->balls[1] + entry->delta);
        BtlDemoCamSetTarget(cam, demo->balls[1]);
        BtlDemoCamUpdate(cam);
        BtlDemoCamUpdate(cam);
        team = SaveGetTeamBakugan(-1);
        spawnPos[2] = 0.0f;
        spawnPos[1] = 0.0f;
        spawnPos[0] = 0.0f;
        spawnPos[3] = 0.0f;
        if (team < 3) {
            if (team >= 2) {
                GfxModelGetNodeWorldPosByIndex((GfxModel *)demo->balls[1], &nodePos, 0);
                CopyVec4(spawnPos, (const float *)&nodePos);
            }
        } else if (team == 10) {
            GfxModelGetNodeWorldPosByIndex((GfxModel *)demo->balls[1], &nodePos, 0x45);
            CopyVec4(spawnPos, (const float *)&nodePos);
        }
        GfxEffectSpawn(g_btlUnitEffectMgr, 0x150, spawnPos);
        main = (BtlMain *)CoreTaskFind(100);
        if (main != NULL) {
            BtlMainUpdateScene(main);
        }
        demo->secondStep = demo->secondStep + 1;
        /* fallthrough */
    case 4:
        if (demo->scenePlayers[2]->finished == 0) {
            return;
        }
        demo->cam.shotId = demo->demoId;
        CoreTaskSetFlags(&demo->scenePlayers[2]->base, 3);
        demo->secondStep = 10;
        /* fallthrough */
    case 10:
        cam = &demo->cam;
        BtlGetStageAmbientColor(stage);
        CopyVec4(pose, stage);
        pose[3] = 1.57079637f;
        ActorBallSetHeading(1.57079637f, (GfxModel *)demo->balls[0]);
        ball = (GfxModel *)demo->balls[0];
        CopyVec4(ball->pos, pose);
        ball = (GfxModel *)demo->balls[0];
        ball->velocity[0] = 0.0f;
        ball->velocity[1] = 0.0f;
        ball->velocity[2] = 0.0f;
        ball->velocity[3] = 0.0f;
        ball = (GfxModel *)demo->balls[0];
        CopyVec4(&ball->data->rootMatrix[12], pose);
        ActorBallSetHeading(pose[3], (GfxModel *)demo->balls[0]);
        BtlDemoCamResetKeysNear(cam);
        demo->cam.fov = 70.0f;
        CoreTaskClearFlags(&demo->scenePlayers[1]->base, 3);
        BtlDemoScenePlayerPrime(&demo->base, demo->scenePlayers[1]);
        for (i = 0; i < 9; i++) {
            entry = &((const VtblEntry *)demo->scenePlayers[1]->base.vtable)[2];
            ((void (*)(void *))entry->fn)((u8 *)demo->scenePlayers[1] + entry->delta);
        }
        CoreTaskSetFlags(&demo->scenePlayers[0]->base, 3);
        demo->bakugan->base.ambient[3] = 0.0f;
        CopyVec4(demo->bakugan->base.pos, pose);
        entry = &((const VtblEntry *)demo->bakugan->base.base.vtable)[7];
        ((void (*)(void *))entry->fn)((u8 *)demo->bakugan + entry->delta);
        demo->flashOn = 0;
        demo->flashAlpha = 0.300000012f;
        entry = &((const VtblEntry *)demo->balls[0]->vtable)[7];
        ((void (*)(void *))entry->fn)((u8 *)demo->balls[0] + entry->delta);
        BtlDemoCamSetTarget(cam, demo->balls[0]);
        BtlDemoCamUpdate(cam);
        BtlDemoCamUpdate(cam);
        main = (BtlMain *)CoreTaskFind(100);
        if (main != NULL) {
            BtlMainUpdateScene(main);
        }
        demo->secondStep = demo->secondStep + 1;
        if (demo->stageModel == NULL) {
            return;
        }
        demo->stageModel->ambient[3] = 1.0f;
        entry = &((const VtblEntry *)demo->stageModel->base.vtable)[7];
        ((void (*)(void *))entry->fn)((u8 *)demo->stageModel + entry->delta);
        g_btlArenaModels[0]->ambient[3] = 0.0f;
        return;

    case 11:
        swing = demo->swingAngle + 0.0350000001f;
        demo->swingAngle = swing;
        if (swing < 0.0f) {
            swing = 0.0f;
        } else if (!(swing <= 1.57079637f)) {
            swing = 1.57079637f;
        }
        demo->swingAngle = swing;
        demo->cam.fov = __builtin_sinf(swing) * -25.0f + 70.0f;
        return;

    default:
        return;
    }
}
