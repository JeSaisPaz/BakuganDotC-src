// bdc 0x0890edf8 UiConfirmDialogMainPhase
#include "bdc.h"

/* Phase 2 of the yes/no confirm dialog (`UiConfirmDialog`), a sub-state machine on `subState`:
   0 waits for the now-loading screen to close; 1 waits for the screen fade, while 1-2 fade the 16
   sprites and the message in (alpha +0.5 per frame up to 1); 3 handles input: in net-play pad mode
   (`unk24`) input waits for NetPlay sync, and profile flag 0x80 skips straight to step 4; Cross
   (0x4000) confirms (sound 0); unless single-button (`unk84 == 1`), Circle (0x2000) moves the cursor
   to button 1 and Left/Right repeat move it with wrap-around; a cursor change plays sound 1, refocuses
   the buttons and restarts the highlight. The highlight sprite [15] grows and fades in a loop while
   waiting. 4-5 burst the highlight on the chosen button; 6 fades everything out (and the backdrop
   fade back from 50 % black if `unk91`); 7 waits for the fade, then stores `cursor + 1` in
   `g_uiConfirmDialogResult` and advances `state`. */

void UiConfirmDialogMainPhase(UiConfirmDialog *dlg)
{
  int oldCursor;
  bool inputEnabled;
  PadState *pad;
  GfxSprite *highlight;
  GfxFader *fader;
  float width;
  float height;
  int i;

  switch (dlg->subState) {
  case 0:
    if (!UiLoadingIsOpen()) {
      dlg->subState++;
    }
    break;
  case 1:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      dlg->subState++;
    }
    /* fallthrough */
  case 2:
    if (dlg->sprites[0]->alpha == 1.0f) {
      dlg->subState++;
    }
    else {
      for (i = 0; i < 16; i++) {
        dlg->sprites[i]->alpha += 0.5f;
        if (!(dlg->sprites[i]->alpha <= 1.0f)) {
          dlg->sprites[i]->alpha = 1.0f;
        }
      }
      dlg->textAlpha += 0.5f;
      if (!(dlg->textAlpha <= 1.0f)) {
        dlg->textAlpha = 1.0f;
      }
    }
    break;
  case 3:
    oldCursor = dlg->cursor;
    inputEnabled = true;
    if (dlg->unk24 != 0) {
      if (NetPlayHasManager()) {
        inputEnabled = false;
        if (NetPlayIsSynced(NetPlayGetManager())) {
          inputEnabled = true;
        }
      }
      if (SaveGetProfileFlag0() && SaveHasProfile() &&
          SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
        dlg->subState++;
        return;
      }
    }
    if (inputEnabled) {
      pad = dlg->pad;
      if ((pad->pressed & 0x4000) != 0) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        dlg->subState++;
      }
      else if (dlg->unk84 != 1) {
        if ((pad->pressed & 0x2000) != 0) {
          dlg->cursor = 1;
        }
        else if ((pad->repeat & 0x80) != 0) {
          dlg->cursor--;
          if (dlg->cursor < 0) {
            dlg->cursor = 1;
          }
        }
        else if ((pad->repeat & 0x20) != 0) {
          dlg->cursor++;
          if (dlg->cursor >= 2) {
            dlg->cursor = 0;
          }
        }
      }
      if (oldCursor != dlg->cursor) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        dlg->pulsePhase = 0.0f;
        UiConfirmDialogSetButtonFocus(dlg, dlg->sprites[8 + oldCursor], dlg->sprites[11 + oldCursor],
                                      dlg->sprites[14], true, -1);
        UiConfirmDialogSetButtonFocus(dlg, dlg->sprites[8 + dlg->cursor],
                                      dlg->sprites[11 + dlg->cursor], dlg->sprites[14], false,
                                      dlg->cursor);
        dlg->sprites[15]->alpha = -1.0f;
      }
    }
    highlight = dlg->sprites[15];
    if (highlight->alpha < 0.0f) {
      if (!(dlg->sprites[14]->addColor[0] <= 0.29f)) {
        GfxSpriteCopy(dlg->sprites[14], highlight);
        dlg->sprites[15]->alpha = 1.0f;
      }
    }
    else {
      width = GfxSpriteGetWidth(highlight) * 1.025f;
      highlight = dlg->sprites[15];
      height = GfxSpriteGetHeight(highlight) * 1.025f;
      UiSpriteSetSize(width, height, highlight);
      dlg->sprites[15]->alpha -= 0.15f;
    }
    break;
  case 4:
    GfxSpriteCopy(dlg->sprites[8 + dlg->cursor], dlg->sprites[15]);
    dlg->sprites[15]->alpha = 0.5f;
    dlg->subState++;
    break;
  case 5:
    width = GfxSpriteGetWidth(dlg->sprites[15]) * 1.05f;
    highlight = dlg->sprites[15];
    height = GfxSpriteGetHeight(highlight) * 1.05f;
    UiSpriteSetSize(width, height, highlight);
    dlg->sprites[15]->alpha -= 0.05f;
    if (dlg->sprites[15]->alpha < 0.0f) {
      dlg->sprites[15]->alpha = 0.0f;
      dlg->subState++;
    }
    break;
  case 6:
    if (dlg->sprites[0]->alpha == 0.0f) {
      if (dlg->unk91 != 0) {
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
        GfxFaderStart(GfxGetActiveFader(), 3);
      }
      dlg->subState++;
    }
    else {
      for (i = 0; i < 16; i++) {
        dlg->sprites[i]->alpha -= 0.5f;
        if (dlg->sprites[i]->alpha < 0.0f) {
          dlg->sprites[i]->alpha = 0.0f;
        }
      }
      dlg->textAlpha -= 0.5f;
      if (dlg->textAlpha < 0.0f) {
        dlg->textAlpha = 0.0f;
      }
    }
    break;
  case 7:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      g_uiConfirmDialogResult = dlg->cursor + 1;
      dlg->subState = 0;
      dlg->state++;
    }
    break;
  }
}
