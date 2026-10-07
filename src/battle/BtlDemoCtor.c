// bdc 0x088feb44 BtlDemoCtor
#include "bdc.h"

/* Constructor of the battle intro demo task `BtlDemo` (task id 0x65, 0x790 bytes): `CoreTaskInit`,
   vtable `g_btlDemoVtbl`, the three LZS package nodes (`IoLzsPackageCtor`) and the demo camera
   (`BtlDemoCamCtor`); sets `g_gfxDisplay->frameSkip = 1`; clears state, step, flash, fade colour,
   particle sets, unit/ball/scene-player pointers, model lists, `g_btlDemoSkipRequest`,
   `g_btlDemoReady` and `g_btlDemoFinished`; `streamId` = -1; zeroes (VFPU bank C720 = 0)
   `flashColour`, `focusPos` (then focusPos.w = 0.6), `g_btlDemoCamVector` and all 21
   `placements`; takes `g_padState` as its pad and clears its `dpadEmulatesStick`; finally
   stores `arg` as `demoId`. Returns `task`. */

CoreTask *BtlDemoCtor(CoreTask *task, u32 arg)
{
    BtlDemo *self = (BtlDemo *)task;
    PadState *pad;
    s32 i;

    CoreTaskInit(task);
    task->vtable = g_btlDemoVtbl;
    IoLzsPackageCtor(&self->packages[0]);
    IoLzsPackageCtor(&self->packages[1]);
    IoLzsPackageCtor(&self->packages[2]);
    BtlDemoCamCtor(&self->cam);
    g_gfxDisplay->frameSkip = 1;
    self->state = 0;
    self->step = 0;
    self->flashAlpha = 0.0f;
    self->flashLevel = 0.0f;
    self->flashFadeSpeed = 0.0f;
    g_btlDemoSkipRequest = 0;
    self->fadeColour.x = 0.0f;
    self->fadeColour.y = 0.0f;
    self->fadeColour.z = 0.0f;
    self->fadeColour.w = 0.0f;
    self->particles0 = NULL;
    self->particles1 = NULL;
    self->flashOn = 0;
    g_btlDemoReady = 0;
    g_btlDemoFinished = 0;
    self->demoId = 0;
    self->bakugan = NULL;
    self->actor = NULL;
    self->balls[0] = NULL;
    self->balls[1] = NULL;
    self->battleCamera = NULL;
    self->endRequest = 0;
    self->streamId = -1;
    self->scenePlayers[0] = NULL;
    self->scenePlayers[1] = NULL;
    self->scenePlayers[2] = NULL;
    self->secondStep = 0;
    self->swingAngle = 0.0f;
    self->clearedF694 = 0.0f;
    self->flashColour[0] = 0.0f;
    self->flashColour[1] = 0.0f;
    self->flashColour[2] = 0.0f;
    self->flashColour[3] = 0.0f;
    self->focusPos[0] = 0.0f;
    self->focusPos[1] = 0.0f;
    self->focusPos[2] = 0.0f;
    self->focusPos[3] = 0.0f;
    self->focusPos[3] = 0.600000024f;
    self->focusValue = 0;
    self->flashBlend = 0.0f;
    g_btlDemoCamVector.x = 0.0f;
    g_btlDemoCamVector.y = 0.0f;
    g_btlDemoCamVector.z = 0.0f;
    g_btlDemoCamVector.w = 0.0f;
    pad = g_padState;
    self->pad = pad;
    pad->dpadEmulatesStick = 0;
    self->mapModels = NULL;
    self->localModels.tail = NULL;
    self->localModels.head = NULL;
    self->localModels.count = 0;
    self->actorList.tail = NULL;
    self->actorList.head = NULL;
    self->actorList.count = 0;
    self->models = NULL;
    self->stageModel = NULL;
    for (i = 0; i < 21; i++) {
        self->placements[i][0] = 0.0f;
        self->placements[i][1] = 0.0f;
        self->placements[i][2] = 0.0f;
        self->placements[i][3] = 0.0f;
    }
    self->demoId = (s32)arg;
    return task;
}
