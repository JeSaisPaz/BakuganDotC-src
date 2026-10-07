// bdc 0x08911c34 UiPauseMainPhase
#include "bdc.h"

/* Main phase of the pause menu, one step per frame (`phaseStep`):
   0  waits until the now-loading screen is gone, plays sound 4 unless the last closed screen
      `g_lastScreenTaskId` is 301/440/460/10020, advances;
   1  fades all sprites in (alpha += 0.25, clamped to 1, mirrored into `hintAlpha`) until sprite
      [12] is opaque, then highlights the cursor entry; after screens 301/371/440/460 also fades
      the screen in from black over 5 frames (step 2), else goes straight to step 3;
   2  waits for the fader;
   3  input: up/down repeat move the cursor (wrapping, sound 6); confirm runs the entry's action
      (`g_uiPauseItemActions``[kind][cursor]`): 13 opens the "DWCommon" dialog (step 20),
      1 closes the menu (phase 4, frame mode 0), anything else stores `leaving` and
      `confirmMsg` (`UiPauseGetSelectedAction`) and goes to step 10 (confirm dialog) or 15
      (no message, -1); cancel (pad bit 0x8 / 0x2000) leaves with `leaving = 0` (step 4) unless
      kind is 6..9. Every frame of step 3 also animates the cursor ripple sprite [36];
   4  hides the help line; when leaving, sets the menu result to the entry's action and fades
      out (to green-tinted 0.8 alpha over 7 frames for result 15, else to black over 10);
      otherwise fades all sprites out and sets menu result 0 when sprite [12] reaches 0;
   5  (result 15 zooms the sprite layer out) waits for the fader, then phase + 1;
   10/11 confirm dialog (task 510) with line `confirmMsg` of "DMPauseEnd"; yes goes to step 4
      (and with `g_profileFlag0` set, kind 6 and action 4 sets profile flag 0x4000 and asks
      the NetPlay session to abort), no back to step 3;
   15/16 entry flash on sprite [36], then step 4;
   20/21 single-button "DWCommon" line 2 dialog, then back to step 3.
   From step 3 on the selected entry frame and the cursor sprite [20] pulse their add colour
   (0.3 - (1 - cos(cursorPulse * pi)) * 0.5 * 0.3, cursorPulse += 0.08 per frame). */

void UiPauseMainPhase(UiPause *self)
{
  GfxFader *fader;
  GfxSprite *sprite;
  UiConfirmDialog *dlg;
  u32 *table;
  s32 count;
  int prev;
  int next;
  int i;
  s32 action;
  float w;
  float h;
  float k;
  float c;

  switch (self->base.phaseStep) {
  case 0:
    if (!UiLoadingIsOpen()) {
      if (g_lastScreenTaskId != 0x1b8 && g_lastScreenTaskId != 0x1cc &&
          g_lastScreenTaskId != 0x12d && g_lastScreenTaskId != 0x2724) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 4, 0, 0);
        }
      }
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 1:
    if (((GfxSprite **)self->base.data)[12]->alpha == 1.0f) {
      UiPauseSetCursorEntry(self, self->cursor, -1);
      if (g_lastScreenTaskId == 0x12d || g_lastScreenTaskId == 0x173 ||
          g_lastScreenTaskId == 0x1b8 || g_lastScreenTaskId == 0x1cc) {
        fader = GfxGetActiveFader();
        fader->start[0] = 0.0f;
        fader->start[1] = 0.0f;
        fader->start[2] = 0.0f;
        fader->start[3] = 1.0f;
        fader = GfxGetActiveFader();
        fader->end[0] = 0.0f;
        fader->end[1] = 0.0f;
        fader->end[2] = 0.0f;
        fader->end[3] = 0.0f;
        GfxFaderStart(GfxGetActiveFader(), 5);
        UiPauseUpdatePlayerBadge(self);
        self->base.phaseStep = self->base.phaseStep + 1;
      } else {
        UiPauseUpdatePlayerBadge(self);
        self->base.phaseStep = self->base.phaseStep + 2;
      }
    } else {
      for (i = 0; i < self->spriteCount; i++) {
        ((GfxSprite **)self->base.data)[i]->alpha = ((GfxSprite **)self->base.data)[i]->alpha + 0.25f;
        if (!(((GfxSprite **)self->base.data)[i]->alpha <= 1.0f)) {
          ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        }
        self->hintAlpha = ((GfxSprite **)self->base.data)[i]->alpha;
      }
    }
    break;
  case 2:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 3:
    if (self->base.pad->repeat & 0x10) {
      prev = self->cursor;
      self->cursor = prev - 1;
      if (self->cursor < 0) {
        self->cursor = self->entryCount - 1;
      }
      UiPauseSetCursorEntry(self, self->cursor, prev);
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 6, 0, 0);
      }
    } else if (self->base.pad->repeat & 0x40) {
      prev = self->cursor;
      next = prev + 1;
      self->cursor = next;
      if (next >= self->entryCount) {
        self->cursor = 0;
        next = 0;
      }
      UiPauseSetCursorEntry(self, next, prev);
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 6, 0, 0);
      }
    } else if (self->base.pad->pressed & 0x4000) {
      action = g_uiPauseItemActions[self->kind][self->cursor];
      if (action == 0xd) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 8, 0, 0);
        }
        self->base.phaseStep = 0x14;
      } else if (action == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 5, 0, 0);
        }
        self->closeTimer = 0;
        self->base.phase = 4;
        UiScreenSetFrameMode(&self->base.base, 0);
        break;
      } else {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 5, 0, 0);
        }
        self->leaving = g_uiPauseItemActions[self->kind][self->cursor] != 0;
        self->confirmMsg = UiPauseGetSelectedAction(self);
        self->base.phaseStep = (self->confirmMsg != -1) ? 10 : 0xf;
      }
    } else if (!(self->base.pad->pressed & 0x100) && !(self->base.pad->pressed & 0x200) &&
               ((self->base.pad->pressed & 0x8) || (self->base.pad->pressed & 0x2000))) {
      if (self->kind != 6 && self->kind != 7 && self->kind != 8 && self->kind != 9) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 7, 0, 0);
        }
        self->leaving = 0;
        self->base.phaseStep = self->base.phaseStep + 1;
      }
    }
    /* cursor ripple: sprite [36] grows and fades, then restarts from the cursor sprite [20] */
    sprite = ((GfxSprite **)self->base.data)[36];
    if (sprite->alpha < 0.0f) {
      if (!(((GfxSprite **)self->base.data)[20]->addColor[0] <= 0.29f)) {
        GfxSpriteCopy(((GfxSprite **)self->base.data)[20], sprite);
        ((GfxSprite **)self->base.data)[36]->alpha = 1.0f;
      }
    } else {
      w = GfxSpriteGetWidth(sprite) * 1.02f;
      sprite = ((GfxSprite **)self->base.data)[36];
      h = GfxSpriteGetHeight(sprite) * 1.02f;
      UiSpriteSetSize(w, h, sprite);
      ((GfxSprite **)self->base.data)[36]->alpha = ((GfxSprite **)self->base.data)[36]->alpha - 0.15f;
    }
    break;
  case 4:
    UiHelpLineHide();
    if (self->leaving != 0) {
      UiSetMenuResult(&self->base, g_uiPauseItemActions[self->kind][self->cursor]);
      if (UiGetMenuResult(&self->base) == 0xf) {
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
        GfxFaderStart(GfxGetActiveFader(), 7);
      } else {
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
        GfxFaderStart(GfxGetActiveFader(), 10);
      }
      self->base.phaseStep = self->base.phaseStep + 1;
    } else if (((GfxSprite **)self->base.data)[12]->alpha == 0.0f) {
      UiSetMenuResult(&self->base, 0);
      self->base.phaseStep = self->base.phaseStep + 1;
    } else {
      for (i = 0; i < self->spriteCount; i++) {
        ((GfxSprite **)self->base.data)[i]->alpha = ((GfxSprite **)self->base.data)[i]->alpha - 0.25f;
        if (((GfxSprite **)self->base.data)[i]->alpha < 0.0f) {
          ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
        }
        self->hintAlpha = ((GfxSprite **)self->base.data)[i]->alpha;
      }
    }
    break;
  case 5:
    if (UiGetMenuResult(&self->base) == 0xf) {
      self->base.spriteLayer->alpha = self->base.spriteLayer->alpha - 0.2f;
      GfxSpriteLayerSetZoom(self->base.spriteLayer->alpha * 0.2f + 0.8f, 0.0f, self->base.spriteLayer, NULL);
    }
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.spriteLayer->alpha = 0.0f;
      self->base.phaseStep = 0;
      self->base.phase = self->base.phase + 1;
    }
    break;
  case 10:
    dlg = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
    dlg->pad = self->base.pad;
    dlg->netPad = self->base.pad;
    table = SaveFindLocalizedBin("DMPauseEnd");
    count = (s32)UiMesTableRelocate(table);
    if (self->confirmMsg < count) {
      UiConfirmDialogSetMessage(((const char **)table)[self->confirmMsg]);
    }
    dlg->cursor = 1;
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 0xb:
    if (CoreTaskExists(0x1fe) == 0) {
      if (UiConfirmDialogGetResult() == 1) {
        self->base.phaseStep = 4;
        if (SaveGetProfileFlag0() != 0 && self->kind == 6 &&
            g_uiPauseItemActions[self->kind][self->cursor] == 4) {
          SaveProfileSetFlags(SaveGetProfile(), 0x4000);
          if (NetPlayHasManager()) {
            NetPlayRequestAbort(NetPlayGetManager());
          }
        }
      } else {
        self->base.phaseStep = 3;
      }
    }
    break;
  case 0xf:
    GfxSpriteCopy(((GfxSprite **)self->base.data)[self->cursor + 5], ((GfxSprite **)self->base.data)[36]);
    ((GfxSprite **)self->base.data)[36]->alpha = 0.5f;
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 0x10:
    w = GfxSpriteGetWidth(((GfxSprite **)self->base.data)[36]) * 1.05f;
    sprite = ((GfxSprite **)self->base.data)[36];
    h = GfxSpriteGetHeight(sprite) * 1.05f;
    UiSpriteSetSize(w, h, sprite);
    ((GfxSprite **)self->base.data)[36]->alpha = ((GfxSprite **)self->base.data)[36]->alpha - 0.08f;
    if (((GfxSprite **)self->base.data)[36]->alpha < 0.0f) {
      ((GfxSprite **)self->base.data)[36]->alpha = 0.0f;
      self->base.phaseStep = 4;
    }
    break;
  case 0x14:
    dlg = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
    table = SaveFindLocalizedBin("DWCommon");
    UiMesTableRelocate(table);
    UiConfirmDialogSetMessage(((const char **)table)[2]);
    dlg->cursor = 0;
    dlg->unk84 = 1;
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 0x15:
    if (CoreTaskExists(0x1fe) == 0) {
      self->base.phaseStep = 3;
    }
    break;
  default:
    break;
  }

  if (self->base.phaseStep >= 3) {
    self->cursorPulse = self->cursorPulse + 0.08f;
    c = __builtin_cosf(self->cursorPulse * 3.1415927f);
    k = 0.3f - (1.0f - c) * 0.5f * 0.3f;
    sprite = ((GfxSprite **)self->base.data)[self->cursor + 5];
    sprite->addColor[0] = k;
    sprite->addColor[1] = k;
    sprite->addColor[2] = 0.0f;
    sprite->addColor[3] = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[20];
    sprite->addColor[0] = k;
    sprite->addColor[1] = k;
    sprite->addColor[2] = k;
    sprite->addColor[3] = 1.0f;
  }
}
