// bdc 0x08924fc4 UiHologramGalleryTweenWindow
#include "bdc.h"

/* Starts the open/close tweens of the confirmation window of the hologram gallery screen
   (steps 9/0x12 of `UiHologramGalleryMainPhase`). The window uses sprites 0xb4..0xb5 (window
   frames), 0xb7..0xba (menu items), 0x80 (list frame) and 0x82 (list item), each with the tween
   of the same index. When `hide` is 0 every sprite first gets flag bit 0 set; the frames
   (0xb4, 0xb5, 0x80) also get their add colour set to (0,0,0,1) and their unlit texture
   (`UiHologramGallerySetWindowFrameLit` / `UiHologramGallerySetItemFrameLit`); the items
   (0xb7..0xba, 0x82) get alpha 0 and are tinted 0.5 when their `disabledItems` bit is set
   (bit 0 for 0xb7/0xb9, bit 1 for 0xb8/0xba, bit 2 for 0x82) and 1.0 otherwise. Then `UiTweenBegin(1.0f, hide, sprite, tween, 1)` starts
   each tween (fade in when `hide` is 0, fade out otherwise). */

void UiHologramGalleryTweenWindow(UiHologramGallery *self, u8 hide)
{
  GfxSprite *sprite;
  float tint;
  u8 bit;
  int i;

  if (hide == 0) {
    for (i = 0xb4; i < 0xb6; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiHologramGallerySetWindowFrameLit(self, ((GfxSprite **)self->base.data)[i], false);
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0xb7; i < 0xbb; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      bit = 0;
      switch (i) {
      case 0xb7:
      case 0xb9:
        bit = 1;
        break;
      case 0xb8:
      case 0xba:
        bit = 2;
        break;
      }
      if (bit != 0) {
        tint = (self->disabledItems & bit) != 0 ? 0.5f : 1.0f;
        sprite->tint[0] = tint;
        sprite->tint[1] = tint;
        sprite->tint[2] = tint;
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0x80; i < 0x81; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiHologramGallerySetItemFrameLit(self, ((GfxSprite **)self->base.data)[i], false);
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0x82; i < 0x83; i++) {
      tint = (self->disabledItems & 4) != 0 ? 0.5f : 1.0f;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = tint;
      sprite->tint[1] = tint;
      sprite->tint[2] = tint;
      sprite->alpha = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
  }
  else {
    for (i = 0xb4; i < 0xb6; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0xb7; i < 0xbb; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0x80; i < 0x81; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
    for (i = 0x82; i < 0x83; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
  }
}
