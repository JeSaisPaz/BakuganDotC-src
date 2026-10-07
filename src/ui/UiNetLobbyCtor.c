// bdc 0x08941178 UiNetLobbyCtor
#include "bdc.h"

/* Constructor of the ad-hoc multiplayer lobby screen, task id 2000 (0x7d0) (base `UiScreenCtor`,
   vtable `g_uiNetLobbyVtable`). Object size 0xa4. Clears the phase/timer state, sets the
   shared-background hand-over flag `g_uiKeepSharedBg`, borrows the sprite table of the network
   menu (task 1999) when it exists, builds the two `UiNetLobbySlideAnim` panels from
   `g_uiNetLobbyPanelSlots` and layout table 14, a text box (depth 20000) and a hidden 0x124 x 0x10
   highlight rect, rewinds an opaque-black fade end colour to transparent, clears profile flags
   0x7eff and sets flag 1, makes sure the message window exists (depth 26000). Returns `self`. */

UiNetLobby *UiNetLobbyCtor(UiNetLobby *self)
{
  bool fromLow;
  UiNetLobbySlideAnim **panels;
  UiNetLobbySlideAnim *anim;
  UiTextBox *box;
  GfxRect *rect;
  GfxFader *fader;
  GfxFader *src;
  UiScreen *menu;
  UiMsgWindow *win;
  s32 sprite;
  s32 i;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = &g_uiNetLobbyVtable;
  self->base.phase = 0;
  self->base.unk30 = 0;
  self->msgTimer = 0;
  self->pulseTimer = 0;
  g_uiKeepSharedBg = 1;
  self->leaving = 0;
  self->subTask = NULL;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  panels = MemAlloc(8, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->panels = panels;
  for (i = 0; i < 2; i++) {
    self->panels[i] = NULL;
  }

  self->base.data = NULL;
  menu = CoreTaskFind(1999);
  if (menu != NULL) {
    self->base.data = UiNetMenuGetSpriteTable(menu);
  }

  for (i = 0; i < 2; i++) {
    sprite = g_uiNetLobbyPanelSlots[i][0];
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    anim = MemAlloc(0x18, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (anim != NULL) {
      UiNetLobbySlideAnimCtor(anim, ((GfxSprite **)self->base.data)[sprite],
                              g_uiLayoutTables[14] + sprite * 10);
    }
    self->panels[i] = anim;
  }

  self->field7c = -1;
  self->joinCursor = -1;
  self->listAlpha = 0.0f;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  box = MemAlloc(0x10, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (box != NULL) {
    UiTextBoxCtor(box);
  }
  self->textBox = box;
  box->packetDepth = 20000.0f;
  self->printerStep = 0;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  rect = MemAlloc(0x50, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (rect != NULL) {
    GfxRectCtor(rect, 0);
  }
  self->highlightRect = rect;
  rect->color[0] = 0.5f;
  rect->color[1] = 0.85f;
  rect->color[2] = 0.5f;
  rect->color[3] = 0.0f;
  GfxRectSetSize((GfxRect *)self->highlightRect, 0x124, 0x10);
  ((GfxRect *)self->highlightRect)->pos[0] = 0.0f;
  ((GfxRect *)self->highlightRect)->pos[1] = 0.0f;
  GfxRectSetVisible((GfxRect *)self->highlightRect, 0);

  if (GfxFaderIsReady()) {
    fader = GfxGetActiveFader();
    /* vcmp.q EQ + bvtl CC[5]: all four lanes of the end colour equal opaque black {0, 0, 0, 1} */
    if (fader->end[0] == 0.0f && fader->end[1] == 0.0f && fader->end[2] == 0.0f &&
        fader->end[3] == 1.0f) {
      fader = GfxGetActiveFader();
      for (i = 0; i < 4; i++) {
        fader->end[i] = 0.0f;
      }
      if (GfxFaderIsFinished(GfxGetActiveFader())) {
        GfxFaderStart(GfxGetActiveFader(), 30);
      } else {
        fader = GfxGetActiveFader();
        src = GfxGetActiveFader();
        for (i = 0; i < 4; i++) {
          fader->color[i] = src->end[i];
        }
      }
    }
  }

  if (SaveHasProfile()) {
    SaveProfileClearFlags(SaveGetProfile(), 0x7eff);
    SaveProfileSetFlags(SaveGetProfile(), 1);
  }
  self->activePanel = 0;
  if (!UiMsgWindowExists()) {
    UiMsgWindowEnsure();
  }
  win = UiMsgWindowGet();
  win->depth = 26000.0f;
  return self;
}
