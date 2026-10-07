// bdc 0x08809b44 UiLanguageSelectAnimateItems
#include "bdc.h"

/* Per-frame animation of the 7 language buttons of `UiLanguageSelect`: for each row i places the
   button `sprites[4+i]` and its two label sprites `sprites[0xc+i]`/`sprites[0x13+i]` at their layout
   depth (layout 0x21 entries 4+i, 0xc+i, 0x13+i via `UiLayoutGetEntry`); the selected row
   (`cursor`) gets the highlight texture `paneOnTex` (others `paneTex`), is lifted 10 units and its
   `itemScale[i]` grows by 0.02 up to 1.1 (others shrink by 0.02 down to 1.0), applied with
   `GfxSpriteSetScaleRotation`; each button's `addColor` is cleared to {0, 0, 0, 0} (bank constant
   C720). Then moves the cursor frame `sprites[0xb]` to the selected row (scale of that
   row), sets its alpha to 1 and pulses its tint with a 60-frame triangle wave of `frameCount`
   (0.5..1.0), and sets the selected button's `addColor` to {tint/2, tint/2, 0, 1}. */

void UiLanguageSelectAnimateItems(void *self)

{
  UiLanguageSelect *ls = (UiLanguageSelect *)self;
  s16 *entry;
  GfxSprite *spr;
  GfxSprite *sel;
  s32 i;
  s32 t;
  float scale;
  float tint;

  for (i = 0; i < 7; i++) {
    entry = UiLayoutGetEntry(0x21, i + 4);
    spr = ls->sprites[4 + i];
    spr->posZ = (float)entry[2];
    if (i == ls->cursor) {
      spr->texture = ls->paneOnTex;
      scale = ls->itemScale[i];
      if (!(scale < 1.1f)) {
        scale = 1.1f;
      }
      else {
        scale = scale + 0.02f;
      }
      ls->itemScale[i] = scale;
      spr->posZ = spr->posZ - 10.0f;
    }
    else {
      spr->texture = ls->paneTex;
      scale = ls->itemScale[i];
      if (scale <= 1.0f) {
        scale = 1.0f;
      }
      else {
        scale = scale - 0.02f;
      }
      ls->itemScale[i] = scale;
    }
    GfxSpriteCenterPivot(spr);
    GfxSpriteSetScaleRotation(spr, ls->itemScale[i], ls->itemScale[i], 0.0f, false);
    /* sv.q of the bank constant C720 = (0, 0, 0, 0) */
    spr->addColor[0] = 0.0f;
    spr->addColor[1] = 0.0f;
    spr->addColor[2] = 0.0f;
    spr->addColor[3] = 0.0f;

    entry = UiLayoutGetEntry(0x21, i + 0xc);
    spr = ls->sprites[0xc + i];
    spr->posZ = (float)entry[2];
    if (i == ls->cursor) {
      spr->posZ = spr->posZ - 10.0f;
    }

    entry = UiLayoutGetEntry(0x21, i + 0x13);
    spr = ls->sprites[0x13 + i];
    spr->posZ = (float)entry[2];
    if (i == ls->cursor) {
      spr->posZ = spr->posZ - 10.0f;
    }
  }

  entry = UiLayoutGetEntry(0x21, ls->cursor + 4);
  spr = ls->sprites[0xb];
  GfxSpriteSetTopLeftPivot(spr);
  spr->posY = (float)entry[1];
  GfxSpriteCenterPivot(spr);
  scale = ls->itemScale[ls->cursor];
  GfxSpriteSetScaleRotation(spr, scale, scale, 0.0f, false);
  entry = UiLayoutGetEntry(0x21, 0xb);
  spr->posZ = (float)(entry[2] - 10);

  t = ls->frameCount % 60;
  if (!(t < 30)) {
    t = 60 - t;
  }
  spr->alpha = 1.0f;
  tint = (float)(30 - t) * 0.033333335f * 0.5f + 0.5f;
  spr->tint[0] = tint;
  spr->tint[1] = tint;
  spr->tint[2] = tint;
  sel = ls->sprites[4 + ls->cursor];
  sel->addColor[0] = tint * 0.5f;
  sel->addColor[1] = tint * 0.5f;
  sel->addColor[2] = 0.0f;
  sel->addColor[3] = 1.0f;
}
