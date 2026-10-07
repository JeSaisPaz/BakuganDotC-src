// bdc 0x088540d0 BtlMainPhaseIntro
#include "bdc.h"

/* Phase-9 update of the battle main task (task id 100, `BtlMainTaskCtor`, vtable `0x08af18f4`;
   the task `BtlGetCameraTask` returns): the pre-battle introduction. Every frame it fades
   `dimColor[3]` (x0.9, 0 once <= 0.005) and runs `BtlMainUpdateScene`, then steps `phaseStep`:
   0 waits for no task 0x6b, clears `g_btlCameraDefaultMode` and starts the stage camera demo
   (`BtlStageCamCreate` for stage `g_scriptGlobalVars[1]`); 1 waits for that demo to end; 2/3 point
   the camera (`BtlCameraSetFollowTarget` on the controller `+0x20`, sized from the model bounding
   box, per-stage orbit yaw/offsets, two `BtlCameraUpdate`s) at the first standing HP object and
   wait 60 frames (step 4; announcer voice 0x2a7c via `SndBgmPlayVoice` at 55, pad bit 8 skips);
   on stage 4 with a standing attribute landmark, steps 5/6/7 do the same for the landmark. Step
   999 (no target, stage 1, or no landmark) and step 8 finish: set `g_btlCameraDefaultMode`, clear
   `busy`, reset the camera (`BtlCameraSetDefaultFollow`) when bit 0x22 of `g_scriptGlobalBits` is
   set (else set `field12`), set `phase`/`drawPhase` to 1 and `phaseStep` to 0. */

/* Bounding-box span of an intro target: diff = box max (`bbox + 4`) - min (`bbox`) in xyz
   (the listing bounces both corners and the difference through stack copies; w is unused),
   returns |diff.xyz| (vdot.t + vsqrt.s). */
static float BtlMainIntroBoxSpan(const float *bbox, float *diff)
{
    const float *max = bbox + 4;

    diff[0] = max[0] - bbox[0];
    diff[1] = max[1] - bbox[1];
    diff[2] = max[2] - bbox[2];
    diff[3] = max[3];
    return __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
}

void BtlMainPhaseIntro(BtlMain *self)
{
    float diff[4];
    ActorStageObjBase *obj;
    float halfHeight;
    float height;
    float dist;
    float len;
    s32 timer;
    bool landmark;

    if (self->dimColor[3] <= 0.005f) {
        self->dimColor[3] = 0.0f;
    } else {
        self->dimColor[3] = self->dimColor[3] * 0.9f;
    }
    BtlMainUpdateScene(self);
    switch (self->phaseStep) {
    case 0:
        if (CoreTaskExists(0x6b) != 0) {
            return;
        }
        g_btlCameraDefaultMode = 0;
        BtlStageCamCreate(g_scriptGlobalVars[1], NULL);
        self->introTimer = 0;
        self->phaseStep = self->phaseStep + 1;
        return;
    case 1:
        if (CoreTaskExists(0x6b) != 0) {
            return;
        }
        self->phaseStep = self->phaseStep + 1;
        /* fall through */
    case 2:
        if (ActorStageObjCountStandingTargets() == 0 || g_scriptGlobalVars[1] == 1) {
            self->phaseStep = 999;
            return;
        }
        self->phaseStep = self->phaseStep + 1;
        /* fall through */
    case 3:
        obj = (ActorStageObjBase *)ActorStageObjGetStandingTarget(0);
        len = BtlMainIntroBoxSpan(obj->base.data->bbox, diff);
        halfHeight = diff[1] * 0.5f;
        if (g_scriptGlobalVars[1] == 10) {
            dist = len * 0.65f;
            height = halfHeight * -0.35f;
        } else {
            dist = len * 1.2f;
            height = halfHeight * -0.1f;
        }
        BtlCameraSetFollowTarget(dist, height, 40.0f, &self->camera, obj, 1, 0, NULL);
        switch (g_scriptGlobalVars[1]) {
        case 2:
        case 4:
        case 9:
            self->camera.followOffset3f4 = 3.14159274f;
            break;
        case 14:
            self->camera.followOffset3f4 = 1.57079637f;
            self->camera.followOffsetY = 250.0f;
            self->camera.followDistanceOffset = -200.0f;
            break;
        case 18:
            self->camera.followOffset3f4 = 1.57079637f;
            break;
        default:
            self->camera.followOffset3f4 = 0.0f;
            break;
        }
        BtlCameraUpdate(&self->camera);
        BtlCameraUpdate(&self->camera);
        self->introTimer = 60;
        self->phaseStep = self->phaseStep + 1;
        return;
    case 4:
        if (self->introTimer == 55) {
            SndBgmPlayVoice(0x2a7c);
        }
        timer = self->introTimer - 1;
        self->introTimer = timer;
        if (timer > 0 && (self->pad->pressed & 8) == 0) {
            return;
        }
        self->phaseStep = self->phaseStep + 1;
        /* fall through */
    case 5:
        landmark = false;
        if (ActorStageObjCountStandingAttrLandmarks() != 0 && g_scriptGlobalVars[1] == 4) {
            landmark = true;
        }
        if (!landmark) {
            if (CoreBitsetTest(0x22, g_scriptGlobalBits)) {
                BtlCameraSetDefaultFollow(&self->camera, 0);
            }
            self->phaseStep = 999;
            return;
        }
        self->phaseStep = self->phaseStep + 1;
        /* fall through */
    case 6:
        obj = (ActorStageObjBase *)ActorStageObjGetStandingAttrLandmark(0);
        len = BtlMainIntroBoxSpan(obj->base.data->bbox, diff);
        halfHeight = diff[1] * 0.5f;
        BtlCameraSetFollowTarget(len * 1.5f, halfHeight * -0.1f, 40.0f, &self->camera, obj, 1, 0,
                                 NULL);
        BtlCameraUpdate(&self->camera);
        self->introTimer = 60;
        self->phaseStep = self->phaseStep + 1;
        return;
    case 7:
        timer = self->introTimer - 1;
        self->introTimer = timer;
        if (timer > 0 && (self->pad->pressed & 8) == 0) {
            return;
        }
        self->phaseStep = self->phaseStep + 1;
        /* fall through */
    default:
        g_btlCameraDefaultMode = 1;
        self->busy = 0;
        if (CoreBitsetTest(0x22, g_scriptGlobalBits)) {
            BtlCameraSetDefaultFollow(&self->camera, 0);
        } else {
            self->field12 = 1;
        }
        self->phase = 1;
        self->drawPhase = 1;
        self->phaseStep = 0;
        return;
    }
}
