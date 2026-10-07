// bdc 0x0884b784 BtlMainUpdateBattleEnd
#include "bdc.h"

/* End-of-battle sequence of the battle main task, run each scene update while the battle-over bit 3
   of `g_scriptGlobalBits` is set. Step `field11` 0: clears the overlay gradient and the end zoom
   and moves to step 10 once every enemy is defeated (`BtlCheckEnemiesDefeated`). Any other step
   first pulses the red overlay gradient `endGradient` (`g_colorRed`; edge alpha
   `l*0.06*(1.5 + sin(phase))`, centre a tenth of it, `l` = step clamped to 1..3, phase += 0.14 per
   frame wrapping at 2*pi), starts the end zoom when none runs (`BtlMainStartEndZoom`, 6 frames,
   amount `l*0.5 + 0.5`, param 0.7) and recentres an unshaken camera. Steps 1..9 wait for BGM player 1
   to stop and the enemies to be defeated, then go to 10; 10 and 11 start a 100-frame fade to white
   (`GfxFaderStart`), clear bit 0x1f and advance; 12 counts `endFrame`, ramps `flashTarget` by
   0.01 per frame, locks the units from frame 70 (`BtlSetControlLockAll`), stops BGM channel 1
   over 0.5 s at frame 80 and at frame 100 clears the flash and goes to step 100; 100 waits for BGM
   player 1 to stop (or be missing), then sets phase/draw phase 2 and phase step 100, stops the BGM
   (`BtlStopBgm`), fades all voices, clears bit 3, sets bit 0x1c and script variables 10, 3 and 8
   to 1. Finally a running end zoom steps the camera screen offset along (cos, sin) of its progress
   by the decaying target. */

void BtlMainUpdateBattleEnd(BtlMain *self)
{
    ScePspFVector4 red;
    float cosine;
    float sine;
    GfxCamera *camera;
    GfxFader *fader;
    float level;
    float base;
    float intensity;
    u32 edgeAlpha;
    u32 centreAlpha;
    u32 packed;
    s32 step;
    s32 i;

    if (!CoreBitsetTest(3, g_scriptGlobalBits)) {
        return;
    }

    if (self->field11 == 0) {
        for (i = 0; i < 3; i++) {
            self->endGradient[i] = 0;
        }
        self->endPulsePhase = 0.0f;
        BtlMainClearEndZoom(self);
        if (BtlCheckEnemiesDefeated(self, false) != 0) {
            self->field11 = 10;
        }
    } else {
        self->endPulsePhase = self->endPulsePhase + 0.139626339f;
        if (!(self->endPulsePhase < 6.28318548f)) {
            self->endPulsePhase = self->endPulsePhase - 6.28318548f;
        }
        step = self->field11;
        if (step < 1) {
            step = 1;
        } else if (step > 3) {
            step = 3;
        }
        level = (float)step;
        base = level * 0.0599999987f;
        sine = __builtin_sinf(self->endPulsePhase);
        intensity = base * 0.5f + base + sine * base;
        red = g_colorRed;
        centreAlpha = (s32)(intensity * 0.100000001f * 255.0f) & 0xff;
        edgeAlpha = (s32)(intensity * 255.0f) & 0xff;
        for (i = 0; i < 3; i++) {
            packed = (u32)VfI2uc(VfF2iz(VfSat0(red.x) * 255.0f, 23)) |
                     ((u32)VfI2uc(VfF2iz(VfSat0(red.y) * 255.0f, 23)) << 8) |
                     ((u32)VfI2uc(VfF2iz(VfSat0(red.z) * 255.0f, 23)) << 16) |
                     ((u32)VfI2uc(VfF2iz(VfSat0(red.w) * 255.0f, 23)) << 24);
            self->endGradient[i] =
                (packed & 0x00ffffff) | (((i & 1) != 0 ? centreAlpha : edgeAlpha) << 24);
        }
        if (self->endZoomFrames == 0) {
            BtlMainStartEndZoom(self, 6, level * 0.5f + 0.5f, 0.699999988f);
            if (g_gfxActiveCamera->shakeFrames == 0) {
                GfxCameraSetScreenOffset(0.0f, 0.0f, g_gfxActiveCamera);
            }
        }

        step = self->field11;
        if (step < 10) {
            if (SndBgmPlayerExists(1) && SndBgmPlayerIsStopped(SndBgmPlayerGet(1)) &&
                BtlCheckEnemiesDefeated(self, false) != 0) {
                self->field11 = 10;
            }
        } else {
            switch (step) {
            case 10:
                self->field11++;
                /* fall through */
            case 11:
                fader = GfxGetActiveFader();
                fader->start[3] = 0.0f;
                fader->start[0] = 1.0f;
                fader->start[1] = 1.0f;
                fader->start[2] = 1.0f;
                fader = GfxGetActiveFader();
                fader->end[0] = 1.0f;
                fader->end[1] = 1.0f;
                fader->end[2] = 1.0f;
                fader->end[3] = 1.0f;
                GfxFaderStart(GfxGetActiveFader(), 100);
                self->endFrame = 0;
                CoreBitsetClear(0x1f, g_scriptGlobalBits);
                self->field11++;
                break;
            case 12:
                self->endFrame++;
                if (self->endFrame >= 70) {
                    BtlSetControlLockAll(1);
                }
                self->flashTarget = (float)self->endFrame * 0.00999999978f;
                if (self->endFrame == 80) {
                    SndBgmCancelChannel(1);
                    SndBgmQueueStop(0.5f, 1);
                }
                if (self->endFrame >= 100) {
                    self->flashTarget = 0.0f;
                    BtlSetControlLockAll(1);
                    self->field11 = 100;
                }
                break;
            case 100:
                if (!SndBgmPlayerExists(1) || SndBgmPlayerIsStopped(SndBgmPlayerGet(1))) {
                    self->phase = 2;
                    self->drawPhase = 2;
                    self->phaseStep = 100;
                    BtlStopBgm();
                    SndManagerFadeOutAllVoices(SndGetManager());
                    CoreBitsetClear(3, g_scriptGlobalBits);
                    g_scriptGlobalVars[10] = 1;
                    CoreBitsetSet(0x1c, g_scriptGlobalBits);
                    g_scriptGlobalVars[3] = 1;
                    g_scriptGlobalVars[8] = 1;
                }
                break;
            default:
                break;
            }
        }
    }

    if (self->endZoomFrames > 0) {
        self->endZoomTarget = self->endZoomTarget - self->endZoomStep;
        self->endZoomFrames--;
        if (self->endZoomFrames == 0) {
            self->endZoomTarget = 0.0f;
        }
        camera = g_gfxActiveCamera;
        cosine = __builtin_cosf(self->endZoomProgress);
        sine = __builtin_sinf(self->endZoomProgress);
        GfxCameraSetScreenOffset(camera->screenOffset[0] + cosine * self->endZoomTarget,
                                 camera->screenOffset[1] + sine * self->endZoomTarget, camera);
        self->endZoomProgress = self->endZoomProgress + self->endZoomParam;
    }
}
