// bdc 0x08902c04 BtlAppearDemoStatePlay
#include "bdc.h"

/* State 1 update of the Bakugan-appearance demo task (task id 0x67, `BtlAppearDemoCreate`,
   `BtlAppearDemoCtor`, update `BtlAppearDemoUpdate`; derived from the battle intro demo). Every
   frame it runs `BtlDemoUpdateObjects`, stores `BtlDemoCheckSkip` into `g_btlDemoSkipRequest`
   (a skip during steps 2..0x13 jumps to step 0x14), resets the camera's `lookHeightBlend` and
   `rollScale` to 1 and then runs the step machine on `step`:
   0 updates the actor and ball models (virtual update, vtable entry 7), sets the face expression
   for scene demo ids 1/7/8/9 and picks the camera near plane per demo id;
   1..3 clear the fade alpha, clear the face at scene frame 0x28, tune the near plane per demo id
   and frame, and once scene player 0 has finished place the ball (stage ambient vector + 5 in x,
   heading pi/2; velocity zeroed from bank C720) and the actor, hand over to scene
   player 1 (`BtlDemoScenePlayerPrime`), update the camera twice and the battle scene (task 100,
   `BtlMainUpdateScene`);
   4 once scene player 1 has finished zeroes `g_btlDemoCamVector`, switches to scene player 2 with
   the camera on the Bakugan (`BtlDemoCamSetTarget`) and hides the actor/Bakugan/balls;
   5 on a skip request deletes the balls, freezes scene player 2 and goes to 0x14;
   0x14 stops both BGM channels (0.5 s), fades out all voices and clears the fade colour;
   0x15/0x16 wait `waitFrames` (10 for demo 7, else none); 0x17 fades `fadeColour.w` up by
   0.06667 per frame and at 1 freezes the scene players, clears the display to black and deletes the
   actor; 0x18 releases the stream file `streamId` and waits until it is gone; 0x19 tears the demo
   down (`BtlDemoFinish`). Other steps do nothing. */

static inline void BtlAppearDemoModelUpdate(GfxModel *model)
{
    const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];

    ((void (*)(void *))update->fn)((u8 *)model + update->delta);
}

static inline void BtlAppearDemoCopy4(float *dst, const float *src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

/* Ball model placement: position and root-matrix translation = pos, velocity zeroed (bank C720). */
static inline void BtlAppearDemoPlaceBall(GfxModel *model, const float *pos)
{
    BtlAppearDemoCopy4(model->pos, pos);
    model->velocity[0] = 0.0f;
    model->velocity[1] = 0.0f;
    model->velocity[2] = 0.0f;
    model->velocity[3] = 0.0f;
    BtlAppearDemoCopy4(&model->data->rootMatrix[12], pos);
}

void BtlAppearDemoStatePlay(BtlAppearDemo *demo)
{
    float ambient[4];
    float basePos[4];
    float ballPos[4];
    BtlDemoCam *cam;
    GfxModel *model;
    BtlMain *battle;
    const VtblEntry *entry;
    float alpha;
    s32 demoId;

    BtlDemoUpdateObjects(&demo->base);
    g_btlDemoSkipRequest = BtlDemoCheckSkip(&demo->base);
    if (demo->base.step >= 2 && demo->base.step < 0x14 && g_btlDemoSkipRequest != 0) {
        demo->base.step = 0x14;
    }
    demo->base.cam.lookHeightBlend = 1.0f;
    demo->base.cam.rollScale = 1.0f;

    switch (demo->base.step) {
    case 0:
        BtlAppearDemoModelUpdate(&demo->base.actor->base);
        BtlAppearDemoModelUpdate((GfxModel *)demo->base.balls[0]);
        if (demo->base.balls[1] != NULL) {
            BtlAppearDemoModelUpdate((GfxModel *)demo->base.balls[1]);
        }
        demo->base.step = demo->base.step + 1;
        switch (demo->scenePlayers[0]->demoId) {
        case 1:
        case 7:
        case 8:
        case 9:
            BtlAppearDemoSetFaceExpression(demo, &demo->base.actor->base);
            break;
        default:
            break;
        }
        switch (demo->scenePlayers[0]->demoId) {
        case 1:
        case 2:
        case 8:
        case 9:
        case 10:
        case 16:
        case 20:
            demo->base.cam.base.nearZ = 3.0f;
            BtlDemoCamUpdate(&demo->base.cam);
            BtlAppearDemoModelUpdate(&demo->base.actor->base);
            BtlAppearDemoModelUpdate(&demo->base.actor->base);
            break;
        case 3:
        case 4:
            demo->base.cam.base.nearZ = 10.0f;
            break;
        case 6:
        case 18:
            demo->base.cam.base.nearZ = 9.0f;
            break;
        case 7:
        case 19:
            demo->base.cam.base.nearZ = 20.0f;
            break;
        case 11:
            demo->base.cam.base.nearZ = 3.0f;
            break;
        case 12:
            demo->base.cam.base.nearZ = 2.0f;
            demo->base.cam.lookHeightBlend = -1.0f;
            break;
        case 14:
            demo->base.cam.base.nearZ = 3.0f;
            break;
        case 15:
            demo->base.cam.base.nearZ = 8.0f;
            break;
        case 21:
            demo->base.cam.base.nearZ = 8.0f;
            break;
        default: /* 5, 13, 17 and out of range */
            demo->base.cam.base.nearZ = 5.0f;
            break;
        }
        return;

    case 1:
        demo->base.step = demo->base.step + 1;
        /* fall through */
    case 2:
        demo->base.fadeColour.w = 0.0f;
        demo->base.step = demo->base.step + 1;
        /* fall through */
    case 3:
        if (demo->scenePlayers[0]->frame == 0x28) {
            BtlAppearDemoClearFaceExpression(demo, demo->base.actor);
        }
        switch (demo->scenePlayers[0]->demoId) {
        case 1:
        case 2:
        case 16:
            if (demo->scenePlayers[0]->frame == 0x32) {
                demo->base.cam.base.nearZ = 5.0f;
            } else if (demo->scenePlayers[0]->frame == 0x50) {
                demo->base.cam.base.nearZ = 2.0f;
            }
            break;
        case 5:
        case 17:
            if (demo->scenePlayers[0]->frame == 0x32) {
                demo->base.cam.base.nearZ = 5.0f;
            } else if (demo->scenePlayers[0]->frame == 0x45) {
                demo->base.cam.base.nearZ = 2.0f;
            }
            break;
        case 6:
        case 18:
            demo->base.cam.rollScale = -1.0f;
            break;
        case 7:
        case 19:
            if (demo->scenePlayers[0]->frame == 0x29) {
                demo->base.cam.base.nearZ = 15.0f;
            } else if (demo->scenePlayers[0]->frame == 0x46) {
                demo->base.cam.base.nearZ = 2.1f;
            }
            break;
        case 8:
        case 20:
            if (demo->scenePlayers[0]->frame == 0x41) {
                demo->base.cam.base.nearZ = 15.0f;
            }
            break;
        case 9:
        case 10:
            if (demo->scenePlayers[0]->frame == 0x47) {
                demo->base.cam.base.nearZ = 5.0f;
            }
            break;
        case 12:
            if (demo->scenePlayers[0]->frame == 0x47) {
                demo->base.cam.base.nearZ = 4.0f;
            }
            demo->base.cam.lookHeightBlend = -1.0f;
            break;
        case 21:
            if (demo->scenePlayers[0]->frame == 0x41) {
                demo->base.cam.base.nearZ = 3.0f;
            }
            break;
        default: /* 3, 4, 11, 13, 14, 15 and out of range */
            break;
        }
        if (!demo->scenePlayers[0]->finished) {
            return;
        }
        /* basePos = stage ambient vector with w = pi/2; ballPos = basePos, x + 5. */
        BtlGetStageAmbientColor(ambient);
        BtlAppearDemoCopy4(basePos, ambient);
        basePos[3] = 1.57079637f;
        BtlAppearDemoCopy4(ballPos, basePos);
        ballPos[0] = ballPos[0] + 5.0f;
        cam = &demo->base.cam;
        model = (GfxModel *)demo->base.balls[0];
        if (demo->base.balls[1] != NULL) {
            model = (GfxModel *)demo->base.balls[1];
        }
        GfxModelForEachMaterial(model, BtlAppearDemoMaterialCallback, NULL);
        ActorBallSetHeading(ballPos[3], model);
        BtlAppearDemoPlaceBall(model, ballPos);
        ActorBallSetHeading(ballPos[3], model);
        model->ambient[3] = 0.0f;
        if (demo->base.balls[1] != NULL) {
            model->ambient[3] = 1.0f;
        }
        demo->base.cam.shotId = demo->scenePlayers[1]->demoId;
        BtlDemoCamResetKeysNear(cam);
        CoreTaskClearFlags(&demo->scenePlayers[1]->base, 3);
        BtlDemoScenePlayerPrime(&demo->base.base, demo->scenePlayers[1]);
        CoreTaskSetFlags(&demo->scenePlayers[0]->base, 3);
        demo->base.actor->base.ambient[3] = 0.0f;
        BtlAppearDemoCopy4(demo->base.actor->base.pos, basePos);
        BtlAppearDemoModelUpdate(&demo->base.actor->base);
        demo->base.flashOn = 0;
        demo->base.flashAlpha = 0.3f;
        if (demo->base.balls[1] != NULL) {
            ActorBallSetHeading(basePos[3], model);
            BtlAppearDemoPlaceBall(model, basePos);
            ActorBallSetHeading(basePos[3], model);
            g_btlArenaModels[0]->ambient[3] = 1.0f;
        } else if (demo->base.stageModel != NULL) {
            demo->base.stageModel->ambient[3] = 1.0f;
            BtlAppearDemoModelUpdate(demo->base.stageModel);
            g_btlArenaModels[0]->ambient[3] = 0.0f;
        }
        BtlAppearDemoModelUpdate(model);
        demoId = demo->scenePlayers[0]->demoId;
        if (demoId < 3 && demoId >= 2) {
            demo->base.cam.fov = 50.0f;
            demo->base.cam.base.nearZ = 4.0f;
            demo->base.cam.base.farZ = 30000.0f;
        }
        BtlDemoCamUpdate(cam);
        BtlDemoCamUpdate(cam);
        battle = CoreTaskFind(100);
        if (battle != NULL) {
            BtlMainUpdateScene(battle);
        }
        demo->base.step = demo->base.step + 1;
        return;

    case 4:
        if (!demo->scenePlayers[1]->finished) {
            return;
        }
        g_btlDemoCamVector.x = 0.0f;
        g_btlDemoCamVector.y = 0.0f;
        g_btlDemoCamVector.z = 0.0f;
        g_btlDemoCamVector.w = 0.0f;
        demo->base.cam.shotId = demo->scenePlayers[2]->demoId;
        cam = &demo->base.cam;
        BtlDemoCamResetKeysFar(cam);
        demo->base.cam.base.nearZ = 5.0f;
        switch (demo->scenePlayers[0]->demoId) {
        case 4:
        case 5:
        case 9:
        case 17:
            demo->base.cam.fov = 50.0f;
            demo->base.cam.base.frustumScale = 1.1f;
            break;
        default:
            break;
        }
        demo->base.actor->base.ambient[3] = 0.0f;
        demo->base.bakugan->base.ambient[3] = 0.0f;
        ((GfxModel *)demo->base.balls[0])->ambient[3] = 0.0f;
        if (demo->base.balls[1] != NULL) {
            ((GfxModel *)demo->base.balls[1])->ambient[3] = 0.0f;
        }
        CoreTaskClearFlags(&demo->scenePlayers[2]->base, 3);
        BtlDemoScenePlayerPrime(&demo->base.base, demo->scenePlayers[2]);
        /* virtual update of scene player 2 (task vtable entry 2) */
        entry = &((const VtblEntry *)demo->scenePlayers[2]->base.vtable)[2];
        ((void (*)(void *))entry->fn)((u8 *)demo->scenePlayers[2] + entry->delta);
        CoreTaskSetFlags(&demo->scenePlayers[1]->base, 3);
        if (demo->base.stageModel != NULL) {
            demo->base.stageModel->ambient[3] = 0.0f;
            BtlAppearDemoModelUpdate(demo->base.stageModel);
            GmoModelSetFlags28(demo->base.stageModel->data, 0x100, 0);
            g_btlArenaModels[0]->ambient[3] = 1.0f;
        }
        BtlDemoCamSetTarget(cam, demo->base.bakugan);
        BtlDemoCamUpdate(cam);
        BtlDemoCamUpdate(cam);
        BtlDemoUpdateObjects(&demo->base);
        demo->base.step = demo->base.step + 1;
        return;

    case 5:
        if (g_btlDemoSkipRequest != 0) {
            CoreObjectDeferDelete(demo->base.balls[0], 0);
            if (demo->base.balls[1] != NULL) {
                CoreObjectDeferDelete(demo->base.balls[1], 0);
            }
            CoreTaskSetFlags(&demo->scenePlayers[2]->base, 3);
            demo->base.step = 0x14;
        }
        return;

    case 0x14:
        SndBgmCancelChannel(0);
        SndBgmQueueStop(0.5f, 0);
        SndBgmCancelChannel(1);
        SndBgmQueueStop(0.5f, 1);
        SndManagerFadeOutAllVoices(SndGetManager());
        demo->base.fadeColour.x = 0.0f;
        demo->base.step = demo->base.step + 1;
        demo->base.fadeColour.y = 0.0f;
        demo->base.fadeColour.z = 0.0f;
        demo->base.fadeColour.w = 0.0f;
        return;

    case 0x15:
        if (demo->base.demoId == 7) {
            demo->waitFrames = 10;
        } else {
            demo->waitFrames = 0;
        }
        demo->base.step = demo->base.step + 1;
        /* fall through */
    case 0x16:
        demo->waitFrames = demo->waitFrames - 1;
        if (demo->waitFrames > 0) {
            return;
        }
        demo->base.step = demo->base.step + 1;
        /* fall through */
    case 0x17:
        alpha = demo->base.fadeColour.w + 0.06667f;
        demo->base.fadeColour.w = alpha;
        if (alpha < 1.0f) {
            return;
        }
        CoreTaskSetFlags(&demo->scenePlayers[0]->base, 3);
        CoreTaskSetFlags(&demo->scenePlayers[1]->base, 3);
        CoreTaskSetFlags(&demo->scenePlayers[2]->base, 3);
        demo->base.fadeColour.w = 1.0f;
        /* display clear colour = black */
        BtlAppearDemoCopy4(g_gfxDisplay->clearColor, &g_colorBlack.x);
        if (demo->base.actor != NULL) {
            CoreObjectDeferDelete(&demo->base.actor->base.base, 0);
            demo->base.actor = NULL;
        }
        demo->base.step = demo->base.step + 1;
        return;

    case 0x18:
        if (demo->base.streamId != -1) {
            SndStreamFileRelease(demo->base.streamId);
            if (SndStreamFileIsLoaded(demo->base.streamId)) {
                return;
            }
            if (!SndStreamFileRelease(-1)) {
                return;
            }
        }
        demo->base.step = demo->base.step + 1;
        return;

    case 0x19:
        BtlDemoFinish(&demo->base.base);
        return;

    default: /* 6..0x13 and anything past 0x19 */
        return;
    }
}
