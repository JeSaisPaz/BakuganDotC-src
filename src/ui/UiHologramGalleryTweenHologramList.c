// bdc 0x08926b8c UiHologramGalleryTweenHologramList
#include "bdc.h"

/* Starts the open/close tweens of the hologram detail page of the hologram gallery screen (task 391,
   `UiHologramGalleryCtor`; steps 0x18/0x1b of `UiHologramGalleryMainPhase`). The page shows the
   hologram `g_hologramParams[helpOrder[helpCursor] * 3 + helpPage]`: attribute icon, picture
   (`UiHologramGallerySetHologramPicture`), price digits (`UiHologramGallerySetPriceDigits`),
   level gauge (`UiHologramGalleryScaleLevel` of `hpLevel`, `UiHologramGallerySetGaugeWidth`), and
   the six attribute slots sliding out from `slotX[0]` to `slotX[k]`.
   Every sprite id gets a fade `UiTweenBegin` (scale 1, flags 1) or a slide `UiTweenBeginSlide`
   (scale 1, flags 5) into `self->tweens[id]`. Hiding just starts the tweens (slides by
   `slotX[0] - slotX[k]`); showing first refreshes the sprites and sets their visible flag. */

void UiHologramGalleryTweenHologramList(UiHologramGallery *self, u8 hide)
{
  GfxSprite *sprite;
  const HologramParam *param;
  u32 cell;
  s32 iconCol;
  s32 iconRow;
  s32 price;
  s32 level;
  s32 id;
  u32 k;
  float x0;

#define SPRITE(n) (((GfxSprite **)self->base.data)[n])

  if (hide == 0) {
    for (id = 0x15; id < 0x16; id++) {
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }
    for (id = 0x1b; id < 0x1c; id++) {
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }
    for (id = 0x18; id < 0x19; id++) {
      SPRITE(id)->flags |= 1;
      GfxSpriteSetCell(SPRITE(id), 0.0f, 1.0f);
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }
    for (id = 0x39; id < 0x3b; id++) {
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }
    for (id = 0x49; id < 0x4b; id++) {
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }

    /* attribute icon: sprite 0x3d by the column byte, 0x3e by the row byte, 3 cells per row */
    cell = UiAttributeGetIconCell(self->helpOrder[self->helpCursor]);
    iconCol = (cell >> 8) & 0xff;
    iconRow = (cell >> 16) & 0xff;
    for (id = 0x3d; id < 0x3f; id++) {
      if (id == 0x3d) {
        GfxSpriteSetCell(SPRITE(id), (float)(iconCol / 3), (float)(iconCol % 3));
      } else {
        GfxSpriteSetCell(SPRITE(id), (float)(iconRow / 3), (float)(iconRow % 3));
      }
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }

    for (id = 0x66; id < 0x67; id++) {
      GfxSpriteSetCell(SPRITE(id), 0.0f, (float)self->helpPage);
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }

    param = &g_hologramParams[self->helpOrder[self->helpCursor] * 3 + self->helpPage];
    price = param->price;
    level = param->hpLevel;
    UiHologramGallerySetPriceDigits(self, price, 0x61, SPRITE(0x66)->posX,
                                    SPRITE(0x66)->posY + 14.0f);

    /* alpha cleared, visible flag left as is */
    for (id = 0x5e; id < 0x62; id++) {
      SPRITE(id)->alpha = 0.0f;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }
    for (id = 0x59; id < 0x5a; id++) {
      UiHologramGallerySetHologramPicture(self, SPRITE(id), (u8)self->helpPage,
                                          self->helpOrder[self->helpCursor]);
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }

    /* level gauge: 0x4e is sized by the scaled level and lit white, 0x4f dimmed */
    for (id = 0x4d; id < 0x51; id++) {
      sprite = SPRITE(id);
      if (id == 0x4e) {
        GfxSpriteSetTopLeftPivot(sprite);
        sprite = SPRITE(id);
        UiHologramGallerySetGaugeWidth(self, sprite,
                                       UiHologramGalleryScaleLevel(self, (u8)level) & 0xff);
        sprite = SPRITE(id);
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 0.0f;
        sprite->alpha = 0.0f;
        sprite = SPRITE(id);
      } else if (id == 0x4f) {
        sprite->tint[0] = 0.3f;
        sprite->tint[1] = 0.3f;
        sprite->tint[2] = 0.0f;
        sprite->alpha = 0.0f;
        sprite = SPRITE(id);
      }
      sprite->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }

    for (id = 0x85; id < 0x86; id++) {
      GfxSpriteSetCell(SPRITE(id), 0.0f, (float)self->helpPage);
      SPRITE(id)->flags |= 1;
      UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    }

    /* slot frames, then the two attribute icon rows of the six slots */
    for (id = 0x86; id < 0x8c; id++) {
      k = (id - 0x86) & 0xff;
      SPRITE(id)->flags |= 1;
      x0 = self->slotX[0];
      SPRITE(id)->posX = x0;
      UiTweenBeginSlide(1.0f, 0.0f, self->slotX[k] - x0, hide, SPRITE(id), &self->tweens[id], 5);
    }
    for (id = 0x8d; id < 0x93; id++) {
      k = (id - 0x8d) & 0xff;
      GfxSpriteSetCell(SPRITE(id), (float)(self->helpOrder[k] / 3), (float)(self->helpOrder[k] % 3));
      SPRITE(id)->flags |= 1;
      x0 = self->slotX[0];
      SPRITE(id)->posX = x0;
      UiTweenBeginSlide(1.0f, 0.0f, self->slotX[k] - x0, hide, SPRITE(id), &self->tweens[id], 5);
    }
    for (id = 0x96; id < 0x9c; id++) {
      k = (id - 0x96) & 0xff;
      GfxSpriteSetCell(SPRITE(id), (float)(self->helpOrder[k] / 3), (float)(self->helpOrder[k] % 3));
      SPRITE(id)->flags |= 1;
      x0 = self->slotX[0];
      SPRITE(id)->posX = x0;
      UiTweenBeginSlide(1.0f, 0.0f, self->slotX[k] - x0, hide, SPRITE(id), &self->tweens[id], 5);
    }
  } else {
    for (id = 0x15; id < 0x16; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x1b; id < 0x1c; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x18; id < 0x19; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x39; id < 0x3b; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x49; id < 0x4b; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x3d; id < 0x3f; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x66; id < 0x67; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x5e; id < 0x62; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x59; id < 0x5a; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x4d; id < 0x51; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x85; id < 0x86; id++) UiTweenBegin(1.0f, hide, SPRITE(id), &self->tweens[id], 1);
    for (id = 0x86; id < 0x8c; id++) {
      k = (id - 0x86) & 0xff;
      UiTweenBeginSlide(1.0f, 0.0f, self->slotX[0] - self->slotX[k], hide, SPRITE(id),
                        &self->tweens[id], 5);
    }
    for (id = 0x8d; id < 0x93; id++) {
      k = (id - 0x8d) & 0xff;
      UiTweenBeginSlide(1.0f, 0.0f, self->slotX[0] - self->slotX[k], hide, SPRITE(id),
                        &self->tweens[id], 5);
    }
    for (id = 0x96; id < 0x9c; id++) {
      k = (id - 0x96) & 0xff;
      UiTweenBeginSlide(1.0f, 0.0f, self->slotX[0] - self->slotX[k], hide, SPRITE(id),
                        &self->tweens[id], 5);
    }
  }

#undef SPRITE
}
