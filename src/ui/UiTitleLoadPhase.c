// bdc 0x08951f10 UiTitleLoadPhase
#include "bdc.h"

/* Continue/load phase (entry 4 of the phase table `0x08a9d3f8` (load `UiTitleLoadAssetsPhase`,
   `UiTitleIntroPhase`, press start, menu, load save, `UiTitleAttractPhase`,
   `UiTitleClosePhase`)) of the title screen (task 200, `UiTitleCtor`; `"title.fab"`,
   `"title_logo.fab"`): fades to a tinted overlay, stops all voices and runs the save-load task
   10010 (`CoreTaskCreate``(0x271a, 100)`) once the BGM has stopped; when it is gone, profile word
   1 decides: 1 (loaded) → BGM off and menu result 1 (phase 6), otherwise fade back to the menu
   (phase 3, sub-state 2). */

void UiTitleLoadPhase(UiScreen *screen)
{
    s32 trackId = -1;
    GfxFader *fader;

    if (SndBgmPlayerExists(0) && !SndBgmPlayerIsStopped(SndBgmPlayerGet(0))) {
        trackId = SndBgmPlayerGetTrackId(SndBgmPlayerGet(0));
    }

    switch (screen->phaseStep) {
    case 0:
        /* fade to a translucent green-grey overlay */
        fader = GfxGetActiveFader();
        fader->start[0] = 0.0f;
        fader->start[1] = 0.0f;
        fader->start[2] = 0.0f;
        fader->start[3] = 0.0f;
        fader = GfxGetActiveFader();
        fader->end[0] = 0.26667f;
        fader->end[1] = 0.53333f;
        fader->end[2] = 0.26667f;
        fader->end[3] = 0.8f;
        GfxFaderSetPreset(GfxGetActiveFader(), 3);
        GfxFaderStart(GfxGetActiveFader(), 15);
        screen->phaseStep++;
        /* fall through */
    case 1:
        GfxFabUpdate(((GfxFab **)screen->bgData)[2]);
        if (trackId == -1 && GfxFaderIsFinished(GfxGetActiveFader())) {
            SndManagerFadeOutAllVoices(SndGetManager());
            CoreTaskCreate(0x271a, 100);
            screen->phaseStep++;
        }
        break;
    case 2:
        if (CoreTaskExists(0x271a) == 0) {
            if (SaveProfileGetWord(SaveGetProfile(), 1) == 1) {
                screen->phaseStep = 20;
            } else {
                screen->phaseStep = 10;
            }
        }
        break;
    case 10:
        GfxFaderSetPreset(GfxGetActiveFader(), 4);
        GfxFaderStart(GfxGetActiveFader(), 15);
        screen->phaseStep++;
        /* fall through */
    case 11:
        if (GfxFaderIsFinished(GfxGetActiveFader())) {
            screen->phase = 3;
            screen->phaseStep = 2;
        }
        break;
    case 20:
        SndBgmCancelChannel(0);
        SndBgmQueueStop(0.5f, 0);
        fader = GfxGetActiveFader();
        fader->start[0] = 0.0f;
        fader->start[1] = 0.0f;
        fader->start[2] = 0.0f;
        fader->start[3] = 0.0f;
        fader = GfxGetActiveFader();
        fader->end[0] = 0.0f;
        fader->end[1] = 0.0f;
        fader->end[2] = 0.0f;
        fader->end[3] = 1.0f;
        GfxFaderStart(GfxGetActiveFader(), 15);
        screen->phaseStep++;
        /* fall through */
    case 21:
        if (GfxFaderIsFinished(GfxGetActiveFader())) {
            screen->phaseStep++;
        }
        break;
    case 22:
        SndStreamFileRelease(0x1a);
        if (!SndStreamFileIsLoaded(0x1a) && SndStreamFileRelease(-1)) {
            screen->phaseStep = 99;
        }
        break;
    case 99:
        screen->phase = 6;
        UiSetMenuResult(screen, 1);
        screen->phaseStep = 0;
        break;
    default:
        break;
    }
    UiTitleNop();
}
