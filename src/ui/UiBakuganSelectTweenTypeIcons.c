// bdc 0x0892f580 UiBakuganSelectTweenTypeIcons
#include "bdc.h"

/* Starts the slide tweens (mode 5, `tweens[0x7e..0x83]`) of the attribute icon sprites 0x7e..0x83
   of the Bakugan select screen (`UiBakuganSelectCtor`). Showing (`hide` = 0): looks up the
   current entry's attribute (`UiBakuganGetAttribute`, `UiAttributeGetIconCell`), sets the
   cells of sprites 0x82 (byte 1 of the packed cell) and 0x83 (byte 2) to (cell / 3, cell % 3),
   makes all six visible on layer 2, moves them 64 left of `spritePos` and slides them back in.
   Hiding: slides each from its current X to 64 left of `spritePos` with fade-out. */

void UiBakuganSelectTweenTypeIcons(UiBakuganSelect *self, u8 hide)
{
  int i;
  int cellA;
  int cellB;
  u32 cells;
  GfxSprite *sprite;

  if (hide == 0) {
    cells = UiAttributeGetIconCell(UiBakuganGetAttribute(self->entries[self->current].bakugan));
    cellA = (cells >> 8) & 0xff;
    cellB = (cells >> 16) & 0xff;
    for (i = 0x7e; i < 0x84; i++) {
      if (i == 0x82) {
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], (float)(cellA / 3),
                         (float)(cellA % 3));
      } else if (i == 0x83) {
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], (float)(cellB / 3),
                         (float)(cellB % 3));
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      ((GfxSprite **)self->base.data)[i]->posX = self->spritePos[i][0] - 64.0f;
      sprite = ((GfxSprite **)self->base.data)[i];
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - sprite->posX, hide, sprite,
                        &self->tweens[i], 5);
    }
  } else {
    for (i = 0x7e; i < 0x84; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      UiTweenBeginSlide(1.0f, 0.0f, sprite->posX - (self->spritePos[i][0] + 64.0f), hide, sprite,
                        &self->tweens[i], 5);
    }
  }
}
