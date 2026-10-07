// bdc 0x0890fc58 UiRepairMainPhase
#include "bdc.h"

/* Phase 2 of the card repair screen (task id 430), stepped by `phaseStep`: 0 waits for the
   now-loading task to close; 1 waits for the fader, then plays the repair jingle (sound
   `0x2c0000a`) and, like step 2, fades the 21 layout sprites and the text alpha `+0x2c0` in by 0.25
   per frame until sprite 0 is fully opaque; 3 waits for the button bit `0x4000` and plays sound 0;
   4 fades them out by 0.25 per frame and, once sprite 0 is transparent, starts a 5-frame fader
   from half-black to clear; 5 waits for that fade and advances `phase`. */

void UiRepairMainPhase(UiScreen *screen)
{
  UiRepair *repair = (UiRepair *)screen;
  GfxFader *fader;
  float alpha;
  int i;

  switch ((u32)screen->phaseStep) {
  case 0:
    if (!UiLoadingIsOpen()) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;

  case 1:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0x2c0000a, 0, 0);
      }
      screen->phaseStep = screen->phaseStep + 1;
    }
    /* fall through */
  case 2:
    if (((GfxSprite **)screen->data)[0]->alpha == 1.0f) {
      screen->phaseStep = screen->phaseStep + 1;
      break;
    }
    for (i = 0; i < 21; i++) {
      ((GfxSprite **)screen->data)[i]->alpha = ((GfxSprite **)screen->data)[i]->alpha + 0.25f;
      if (!(((GfxSprite **)screen->data)[i]->alpha <= 1.0f)) {
        ((GfxSprite **)screen->data)[i]->alpha = 1.0f;
      }
    }
    alpha = repair->alpha + 0.25f;
    repair->alpha = alpha;
    if (!(alpha <= 1.0f)) {
      repair->alpha = 1.0f;
    }
    break;

  case 3:
    if ((screen->pad->pressed & 0x4000) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;

  case 4:
    if (((GfxSprite **)screen->data)[0]->alpha == 0.0f) {
      fader = GfxGetActiveFader();
      fader->start[0] = 0.0f;
      fader->start[1] = 0.0f;
      fader->start[2] = 0.0f;
      fader->start[3] = 0.5f;
      fader = GfxGetActiveFader();
      fader->end[0] = 0.0f;
      fader->end[1] = 0.0f;
      fader->end[2] = 0.0f;
      fader->end[3] = 0.0f;
      GfxFaderStart(GfxGetActiveFader(), 5);
      screen->phaseStep = screen->phaseStep + 1;
      break;
    }
    for (i = 0; i < 21; i++) {
      ((GfxSprite **)screen->data)[i]->alpha = ((GfxSprite **)screen->data)[i]->alpha - 0.25f;
      if (((GfxSprite **)screen->data)[i]->alpha < 0.0f) {
        ((GfxSprite **)screen->data)[i]->alpha = 0.0f;
      }
    }
    alpha = repair->alpha - 0.25f;
    repair->alpha = alpha;
    if (alpha < 0.0f) {
      repair->alpha = 0.0f;
    }
    break;

  case 5:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      screen->phaseStep = 0;
      screen->phase = screen->phase + 1;
    }
    break;
  }
}
