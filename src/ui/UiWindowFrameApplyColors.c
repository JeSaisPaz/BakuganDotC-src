// bdc 0x089fea84 UiWindowFrameApplyColors
#include "bdc.h"

/* Applies the colours of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`) (vtable `+0x34`): eases the brightness `+0x100` toward 1.0 (highlighted, flag 4) or
   0.5, then sets the tint of the 8 border sprites to `borderColor * brightness` and of the fill
   sprite (`+0xac`) to `fillColor * brightness`, with alpha = `+0xfc * colour alpha`.
   The listing scales the whole colour quad (lane 3 lands in the sprite's alpha) and then
   overwrites the alpha, so only the final alpha is written here. */

void UiWindowFrameApplyColors(UiWindowFrame *self)

{
  GfxSprite *sprite;
  GfxSprite *fill;
  int i;
  float alpha;
  float target;
  float bright;
  float borderAlpha;
  float fillAlpha;

  sprite = ((GfxSpriteLayer *)self)->head;
  if ((self->flags & 4) == 0) {
    target = 0.5f;
    if (!(self->brightness <= 0.51f)) {
      target = self->brightness + (0.5f - self->brightness) * 0.4f;
    }
    alpha = self->alpha;
  }
  else {
    target = 1.0f;
    if (self->brightness < 0.99f) {
      target = self->brightness + (1.0f - self->brightness) * 0.4f;
    }
    alpha = self->alpha;
  }
  self->brightness = target;
  borderAlpha = alpha * self->borderColor[3];
  i = 0;
  do {
    bright = self->brightness;
    sprite->tint[0] = self->borderColor[0] * bright;
    sprite->tint[1] = self->borderColor[1] * bright;
    sprite->tint[2] = self->borderColor[2] * bright;
    sprite->alpha = borderAlpha;
    i = i + 1;
    sprite = sprite->next;
  } while (i < 8);
  fillAlpha = self->alpha * self->fillColor[3];
  fill = self->fill;
  bright = self->brightness;
  fill->tint[0] = self->fillColor[0] * bright;
  fill->tint[1] = self->fillColor[1] * bright;
  fill->tint[2] = self->fillColor[2] * bright;
  self->fill->alpha = fillAlpha;
}
