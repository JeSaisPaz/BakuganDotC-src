// bdc 0x089511b8 UiTitleAttractPhase
#include "bdc.h"

/* Phase 5 of `UiTitle`, entered after 40 s without input on the press-start screen:
   steps the `title_repert` animation, fades out over 15 frames, then sets bit 31 of profile word 0
   (`SaveProfileSetWord`), stops the BGM (0.5 s), reports menu result 3 and goes to phase 6. */

void UiTitleAttractPhase(UiScreen *screen)

{
  GfxFader *fader;
  SaveProfile *profile;
  u32 word;
  
  GfxFabUpdate(((GfxFab **)screen->bgData)[2]);
  if (screen->phaseStep == 0) {
    fader = GfxGetActiveFader();
    GfxFaderSetPreset(fader,1);
    fader = GfxGetActiveFader();
    GfxFaderStart(fader,0xf);
    screen->phaseStep = screen->phaseStep + 1;
  }
  else {
    fader = GfxGetActiveFader();
    if (GfxFaderIsFinished(fader)) {
      profile = SaveGetProfile();
      word = SaveProfileGetWord(profile,0);
      profile = SaveGetProfile();
      SaveProfileSetWord(profile,0,word | 0x80000000);
      SndBgmQueueStop(0.5f,0);
      UiSetMenuResult(screen,3);
      screen->phase = 6;
      screen->phaseStep = 0;
    }
  }
  return;
}

