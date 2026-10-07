// bdc 0x0890ea3c UiConfirmDialogSetupPhase
#include "bdc.h"

/* Phase 1 of the yes/no confirm dialog (`UiConfirmDialog`). On its first step (`subState == 0`):
   forces the cursor to 0 when `unk84` is 1, creates the layout-15 sprites into the sprite table,
   allocates and adds a 16th (highlight) sprite, hides all 16 (visible bit cleared, alpha 0), then
   shows the panel [7], both buttons ([8]/[9] frames, [11]/[12] labels) when `unk84` is 0, or a single
   centred button (button 1's frame/label copied over button 0, the cursor shifted by the same offset)
   otherwise; centres, resets and UV-insets the button and cursor sprites, stores the two cursor x
   positions (`buttonX`, 103 px apart), focuses the current button and sets the highlight alpha to -1.
   Then, if `unk91` is set, fades the screen from clear to 50 % black over 3 frames. Always advances
   to the next state with `subState` reset. */

void UiConfirmDialogSetupPhase(UiConfirmDialog *dlg)
{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *highlight;
  GfxSprite *cursorSprite;
  GfxFader *fader;
  int i;

  if (dlg->subState == 0) {
    if (dlg->unk84 == 1) {
      dlg->cursor = 0;
    }
    UiLayoutCreateSprites(dlg->spriteLayer, dlg->sprites, 15);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprite = MemAlloc(0x160, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    highlight = NULL;
    if (sprite != NULL) {
      GfxSpriteCtor(sprite);
      highlight = sprite;
    }
    dlg->sprites[15] = highlight;
    GfxSpriteLayerAdd(dlg->spriteLayer, (CoreObject *)dlg->sprites[15]);
    for (i = 0; i < 16; i++) {
      dlg->sprites[i]->flags &= ~1u;
      dlg->sprites[i]->alpha = 0.0f;
    }
    dlg->sprites[7]->flags |= 1;
    if (dlg->unk84 == 0) {
      dlg->sprites[9]->flags |= 1;
      dlg->sprites[12]->flags |= 1;
    }
    else {
      dlg->sprites[14]->posX = dlg->sprites[14]->posX + (dlg->sprites[10]->posX - dlg->sprites[8]->posX);
      GfxSpriteCopy(dlg->sprites[10], dlg->sprites[8]);
      GfxSpriteCopy(dlg->sprites[13], dlg->sprites[11]);
    }
    dlg->sprites[8]->flags |= 1;
    dlg->sprites[11]->flags |= 1;
    for (i = 0; i < 2; i++) {
      GfxSpriteCenterPivot(dlg->sprites[8 + i]);
      GfxSpriteSetScaleRotation(dlg->sprites[8 + i], 1.0f, 1.0f, 0.0f, false);
      GfxSpriteInsetUv(0.5f, dlg->sprites[8 + i]);
      GfxSpriteCenterPivot(dlg->sprites[11 + i]);
      GfxSpriteSetScaleRotation(dlg->sprites[11 + i], 1.0f, 1.0f, 0.0f, false);
      GfxSpriteInsetUv(0.5f, dlg->sprites[11 + i]);
    }
    GfxSpriteCenterPivot(dlg->sprites[14]);
    GfxSpriteSetScaleRotation(dlg->sprites[14], 1.0f, 1.0f, 0.0f, false);
    GfxSpriteInsetUv(0.5f, dlg->sprites[14]);
    cursorSprite = dlg->sprites[14];
    dlg->buttonX[0] = cursorSprite->posX;
    dlg->buttonX[1] = cursorSprite->posX + 103.0f;
    UiConfirmDialogSetButtonFocus(dlg, dlg->sprites[8 + dlg->cursor], dlg->sprites[11 + dlg->cursor],
                                  cursorSprite, false, dlg->cursor);
    dlg->sprites[15]->alpha = -1.0f;
    dlg->subState++;
  }
  if (dlg->unk91 != 0) {
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.5f;
    GfxFaderStart(GfxGetActiveFader(), 3);
  }
  dlg->subState = 0;
  dlg->state++;
}
