// bdc 0x089013a0 BtlDemoStatePlay
#include "bdc.h"

/* State 1 update of the battle intro demo task (task id 0x65, `BtlDemoCtor`, update
   `BtlDemoUpdate` / draw `BtlDemoDraw` on the state `+0x65c`): runs the demo
   (`BtlDemoUpdateObjects`, `BtlDemoUpdateSecondUnit`) and then a step machine on `step`:
   0 fades `fadeColour.w` down and polls `BtlDemoCheckSkip` (into `g_btlDemoSkipRequest`); on a
   skip/end it stops BGM channel 0 (and channel 1 when skipped by the player), 10..12 wait for the
   BGM players to stop and queue the battle BGM (0x19 in rule mode 2, else 0x18 when window kind 4
   is active; step 0x32 when none was queued); 13/20 wait for task 0x2726 to end and window kind 10,
   then go to 0x32 (or 100 in rule mode 1 with window kind 4 active); 0x32 creates the pause menu
   task 0x19a (menu kind 6..9 from the rule mode and profile words 0x2e/0x2b); 0x33 waits for it to
   close (or removes it early when `SaveGetProfileFlag0` is set and profile word 0 has bit 0x80)
   and moves to 100; 100 stops both BGM channels and fades out all voices; 101 fades
   `fadeColour.w` up to 1, copies `fadeColour` into the battle camera's `dimColor` (via a stack
   temp), hides/freezes the scene players, sets the display clear colour to `g_colorBlack`;
   0x66 releases the stream file `streamId`; 0x67 tears the demo
   down (`BtlDemoFinish`). */

void BtlDemoStatePlay(BtlDemo *demo)
{
    SaveProfile *profile;
    UiPause *pause;
    BtlMain *camera;
    ScePspFVector4 tmp;
    float alpha;
    int started;
    int step;

    BtlDemoUpdateObjects(demo);
    BtlDemoUpdateSecondUnit(demo);
    step = demo->step;

    if (step >= 0x34) {
        if (step >= 0x66) {
            if (step == 0x66) {
                goto release_stream;
            }
            if (step == 0x67) {
                BtlDemoFinish(&demo->base);
                demo->step = demo->step + 1;
            }
            return;
        }
        if (step < 100) {
            return;
        }
        if (step == 100) {
            SndBgmCancelChannel(0);
            SndBgmQueueStop(0.4f, 0);
            SndBgmCancelChannel(1);
            SndBgmQueueStop(0.4f, 1);
            SndManagerFadeOutAllVoices(SndGetManager());
            demo->step = demo->step + 1;
        }
        /* step 101: fade up */
        alpha = demo->fadeColour.w + 0.06667f;
        demo->fadeColour.w = alpha;
        if (alpha < 1.0f) {
            return;
        }
        demo->fadeColour.w = 1.0f;
        camera = (BtlMain *)BtlGetCameraTask();
        tmp = demo->fadeColour;
        camera->dimColor[0] = tmp.x;
        camera->dimColor[1] = tmp.y;
        camera->dimColor[2] = tmp.z;
        camera->dimColor[3] = tmp.w;
        CoreTaskSetFlags(&demo->scenePlayers[0]->base, 3);
        if (demo->scenePlayers[1] != NULL) {
            CoreTaskSetFlags(&demo->scenePlayers[1]->base, 3);
        }
        g_gfxDisplay->clearColor[0] = g_colorBlack.x;
        g_gfxDisplay->clearColor[1] = g_colorBlack.y;
        g_gfxDisplay->clearColor[2] = g_colorBlack.z;
        g_gfxDisplay->clearColor[3] = g_colorBlack.w;
        demo->step = demo->step + 1;

release_stream:
        if (demo->streamId != -1) {
            SndStreamFileRelease(demo->streamId);
            if (SndStreamFileIsLoaded(demo->streamId)) {
                return;
            }
            if (!SndStreamFileRelease(-1)) {
                return;
            }
        }
        demo->step = demo->step + 1;
        return;
    }

    if (step >= 0x15) {
        if (step < 0x32) {
            return;
        }
        if (step == 0x32) {
            pause = (UiPause *)CoreTaskCreate(0x19a, 100);
            if (pause != NULL) {
                if (g_scriptGlobalVars[8] == 2) {
                    UiPauseSetMenuKind(pause, 6);
                } else {
                    UiPauseSetMenuKind(pause, 7);
                    profile = SaveGetProfile();
                    if (SaveProfileGetWord(profile, 0x2e) != 0) {
                        profile = SaveGetProfile();
                        if (SaveProfileGetWord(profile, 0x2b) == 1) {
                            UiPauseSetMenuKind(pause, 8);
                        } else {
                            profile = SaveGetProfile();
                            if (SaveProfileGetWord(profile, 0x2b) == 2) {
                                UiPauseSetMenuKind(pause, 9);
                            }
                        }
                    }
                }
            }
            demo->step = demo->step + 1;
            return;
        }
        /* step 0x33: wait for the pause menu task */
        if (CoreTaskExists(0x19a) == 0) {
            demo->step = 100;
        }
        if (SaveGetProfileFlag0() == 0) {
            return;
        }
        if (!SaveHasProfile()) {
            return;
        }
        profile = SaveGetProfile();
        if (!SaveProfileHasFlags(profile, 0x80)) {
            return;
        }
        if (demo->step == 100) {
            return;
        }
        CoreTaskRemoveById(0x19a);
        demo->step = 100;
        return;
    }

    switch (step) {
    case 0:
        if (demo->fadeColour.w <= 0.005f) {
            alpha = 0.0f;
        } else {
            alpha = demo->fadeColour.w * 0.9f;
        }
        demo->fadeColour.w = alpha;
        g_btlDemoSkipRequest = BtlDemoCheckSkip(demo);
        if (g_btlDemoSkipRequest == 0) {
            return;
        }
        SndBgmCancelChannel(0);
        SndBgmQueueStop(0.5f, 0);
        if (g_btlDemoSkipRequest == 2) {
            SndBgmCancelChannel(1);
            SndBgmQueueStop(1.0f, 1);
        }
        demo->endTimer = 0;
        demo->step = 10;
        demo->fadeColour.x = 0.0f;
        demo->fadeColour.y = 0.0f;
        demo->fadeColour.z = 0.0f;
        demo->fadeColour.w = 0.0f;
        /* fall through */
    case 10:
        demo->step = 11;
        /* fall through */
    case 11:
        demo->endTimer = demo->endTimer - 1;
        if (demo->endTimer > 0) {
            return;
        }
        demo->step = demo->step + 1;
        /* fall through */
    case 12:
        started = 0;
        if (SndBgmPlayerExists(0)) {
            if (!SndBgmPlayerIsStopped(SndBgmPlayerGet(0))) {
                return;
            }
        }
        if (g_btlDemoSkipRequest != 2 && SndBgmPlayerExists(1)) {
            if (!SndBgmPlayerIsStopped(SndBgmPlayerGet(1))) {
                return;
            }
        }
        if (g_scriptGlobalVars[8] == 2) {
            SndBgmQueuePlay(0, 0x19, 1, 0);
            started = 1;
        } else if (UiGetWindowActive(4)) {
            SndBgmQueuePlay(0, 0x18, 1, 0);
            started = 1;
        }
        if (started) {
            UiSetWindowActive(9, 1);
            demo->step = demo->step + 1;
        } else {
            demo->step = 0x32;
        }
        return;
    case 13:
        demo->step = 0x14;
        /* fall through */
    case 20:
        if (CoreTaskExists(0x2726) != 0) {
            return;
        }
        if (!UiGetWindowActive(10)) {
            return;
        }
        started = 1;
        if (g_scriptGlobalVars[8] == 1 && UiGetWindowActive(4)) {
            started = 0;
        }
        demo->step = started ? 0x32 : 100;
        return;
    default:
        return;
    }
}
