// bdc 0x0890e834 UiConfirmDialogSetButtonFocus
#include "bdc.h"

/* Highlights or un-highlights a button of the confirm dialog (`UiConfirmDialog`).
   `unfocus == false` → grows the frame, label and cursor sprites by 10 % (`UiSpriteSetSize`),
   sets the frame texture `cus_mi_bo_1`, shows the cursor (flags bit 0) and, unless `index` is -1,
   moves it to `buttonX[index]`; `unfocus == true` → shrinks them back (x 1/1.1), sets texture
   `cus_mi_bo_2` and hides the cursor. `dlg` is the dialog (callers hold it as a UiScreen). */

void UiConfirmDialogSetButtonFocus(void *dlg, GfxSprite *frame, GfxSprite *label, GfxSprite *cursor, bool unfocus, int index)
{
  UiConfirmDialog *self = dlg;
  float w;
  float h;

  if (unfocus) {
    w = GfxSpriteGetWidth(frame) * 0.9090909f;
    h = GfxSpriteGetHeight(frame);
    UiSpriteSetSize(w, h * 0.9090909f, frame);
    frame->texture = GfxFindTexture("cus_mi_bo_2");
    w = GfxSpriteGetWidth(label) * 0.9090909f;
    h = GfxSpriteGetHeight(label);
    UiSpriteSetSize(w, h * 0.9090909f, label);
    w = GfxSpriteGetWidth(cursor) * 0.9090909f;
    h = GfxSpriteGetHeight(cursor);
    UiSpriteSetSize(w, h * 0.9090909f, cursor);
    cursor->flags &= ~1u;
  } else {
    w = GfxSpriteGetWidth(frame) * 1.1f;
    h = GfxSpriteGetHeight(frame);
    UiSpriteSetSize(w, h * 1.1f, frame);
    frame->texture = GfxFindTexture("cus_mi_bo_1");
    w = GfxSpriteGetWidth(label) * 1.1f;
    h = GfxSpriteGetHeight(label);
    UiSpriteSetSize(w, h * 1.1f, label);
    w = GfxSpriteGetWidth(cursor) * 1.1f;
    h = GfxSpriteGetHeight(cursor);
    UiSpriteSetSize(w, h * 1.1f, cursor);
    cursor->flags |= 1u;
    if (index != -1) {
      cursor->posX = self->buttonX[index];
    }
  }
}
