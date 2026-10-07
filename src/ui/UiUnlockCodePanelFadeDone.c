// bdc 0x089931c0 UiUnlockCodePanelFadeDone
#include "bdc.h"

/* Advances the keyboard panel fade of `UiUnlockCode` by 1/8 (`panelFadeT`) on
   data sprites 1..40. Fading out (`out` != 0) eases their alpha 1 - t*t down and snaps it to 0
   once t reaches 1. Fading in eases alpha 1 - (t-1)^2 up and snaps it to 1 once t reaches 1,
   except sprites 8, 9 and 12 (alpha at half that curve) and sprite 13 (alpha 1, tint at half the
   curve), which never count as finished. Returns true when at least one sprite reached the end of
   the fade; on a finished fade-in it also shows sprites 5..9 and 36..40 again and re-places the
   input caret (sprite 26, at 104.5, 33.5, colour g_unlockCodeColorHighlight, shown). */

bool UiUnlockCodePanelFadeDone(UiUnlockCode *self, u8 out)

{
  GfxSprite *s;
  u8 done;
  int i;
  float d;
  float v;

  done = 0;
  self->panelFadeT = self->panelFadeT + 0.125f;
  if (out == 0) {
    for (i = 1; i < 41; i++) {
      if (i == 8 || i == 9 || i == 12) {
        d = self->panelFadeT - 1.0f;
        ((GfxSprite **)self->base.data)[i]->alpha = (1.0f - d * d) * 0.5f;
      } else if (i == 13) {
        d = self->panelFadeT - 1.0f;
        s = ((GfxSprite **)self->base.data)[i];
        s->alpha = 1.0f;
        v = (1.0f - d * d) * 0.5f;
        s->tint[0] = v;
        s->tint[1] = v;
        s->tint[2] = v;
      } else {
        d = self->panelFadeT - 1.0f;
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f - d * d;
        if (!(self->panelFadeT < 1.0f)) {
          done++;
          ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        }
      }
    }
  } else {
    for (i = 1; i < 41; i++) {
      ((GfxSprite **)self->base.data)[i]->alpha = 1.0f - self->panelFadeT * self->panelFadeT;
      if (!(self->panelFadeT < 1.0f)) {
        done++;
        ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
      }
    }
  }
  if (out == 0 && done != 0) {
    for (i = 5; i < 10; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
    }
    for (i = 36; i < 41; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
    }
    ((GfxSprite **)self->base.data)[26]->posX = 104.5f;
    ((GfxSprite **)self->base.data)[26]->posY = 33.5f;
    s = ((GfxSprite **)self->base.data)[26];
    s->tint[0] = g_unlockCodeColorHighlight.x;
    s->tint[1] = g_unlockCodeColorHighlight.y;
    s->tint[2] = g_unlockCodeColorHighlight.z;
    s->alpha = g_unlockCodeColorHighlight.w;
    ((GfxSprite **)self->base.data)[26]->flags |= 1;
  }
  return done != 0;
}
