// bdc 0x08993004 UiUnlockCodeStartPanelFade
#include "bdc.h"

/* Prepares the keyboard panel fade of `UiUnlockCode` (`panelFadeT` = 0).
   Fading out (`out` != 0) clears both text layers (`keyText`, `digitText`) and hides sprites
   5..9, 36..40 and 26; fading in resets the OK/back sprite colours (sprites 25 and 24 get
   `g_unlockCodeColorDimGreen`, then alpha 0), clears the add-colour of sprite 13 (alpha 1, texture
   slot 1) and hides sprites 27, 32 and 33. Advanced by `UiUnlockCodePanelFadeDone`. */

void UiUnlockCodeStartPanelFade(UiUnlockCode *self, u8 out)

{
  GfxSprite *a;
  GfxSprite *b;
  GfxSprite *s;
  UiTextPrinter *text;
  int i;

  if (out == 0) {
    a = ((GfxSprite **)self->base.data)[24];
    b = ((GfxSprite **)self->base.data)[25];
    /* quad copy global -> sprite 25 tint+alpha -> sprite 24 tint+alpha */
    b->tint[0] = g_unlockCodeColorDimGreen.x;
    b->tint[1] = g_unlockCodeColorDimGreen.y;
    b->tint[2] = g_unlockCodeColorDimGreen.z;
    b->alpha = g_unlockCodeColorDimGreen.w;
    a->tint[0] = b->tint[0];
    a->tint[1] = b->tint[1];
    a->tint[2] = b->tint[2];
    a->alpha = b->alpha;
    ((GfxSprite **)self->base.data)[24]->alpha = 0.0f;
    ((GfxSprite **)self->base.data)[25]->alpha = 0.0f;
    s = ((GfxSprite **)self->base.data)[13];
    s->addColor[0] = 0.0f;
    s->addColor[1] = 0.0f;
    s->addColor[2] = 0.0f;
    s->addColor[3] = 1.0f;
    ((GfxSprite **)self->base.data)[13]->textureSlot = 1;
    ((GfxSprite **)self->base.data)[27]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[32]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[33]->flags &= ~1u;
    self->panelFadeT = 0.0f;
  }
  else {
    text = self->keyText;
    GfxSpriteLayerClear(&text->layer);
    text->glyphs = NULL;
    text = self->digitText;
    GfxSpriteLayerClear(&text->layer);
    text->glyphs = NULL;
    for (i = 5; i < 10; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    for (i = 36; i < 41; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    for (i = 26; i < 27; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    self->panelFadeT = 0.0f;
  }
}
