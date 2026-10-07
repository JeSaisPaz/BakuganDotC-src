// bdc 0x0892ee84 UiBakuganSelectShowNamePanel
#include "bdc.h"

/* Shows (`hide == 0`) or hides the name panel of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371) for the current entry (`current`, owned list `entries`).
   Clears the 4-byte record at `namePanelOn` and stores `on`/`hide` into it. Showing: sprite 6
   (attribute row, `UiBakuganSetAttributeRow`, mirrored) is made visible at alpha 0, placed 32 px
   left of its home X and given a slide-in tween (`UiTweenBeginSlide`, tween 6, flags 7); the
   name sprite 0 (`UiBakuganSetFullNameTexture`) and the attribute cell sprite 7
   (`UiBakuganSetAttributeCell`) follow sprite 6: same alpha, positioned at sprite 6's position
   plus `namePanelOffset` scaled by sprite 6's scale, and given its scale. Hiding: only starts the
   fade-out tween of sprite 6 (`UiTweenBegin`, flags 1). */

void UiBakuganSelectShowNamePanel(UiBakuganSelect *self, u8 on, u8 hide)
{
  GfxSprite **sprites;
  GfxSprite *sprite;

  memset(&self->namePanelOn, 0, 4);
  self->namePanelFadeOut = hide;
  self->namePanelOn = on;
  sprites = (GfxSprite **)self->base.data;
  sprite = sprites[6];
  if (self->namePanelFadeOut == 0) {
    UiBakuganSetAttributeRow(sprite, self->entries[self->current].bakugan);
    GfxSpriteFlipU(((GfxSprite **)self->base.data)[6]);
    sprites = (GfxSprite **)self->base.data;
    sprites[6]->flags |= 1;
    sprites[6]->alpha = 0.0f;
    sprites[6]->layerMask = 2;
    sprites[6]->posX = self->spritePos[6][0] - 32.0f;
    UiTweenBeginSlide(1.2f, 0.0f, 32.0f, hide, sprites[6], &self->tweens[6], 7);

    UiBakuganSetFullNameTexture(((GfxSprite **)self->base.data)[0],
                                self->entries[self->current].bakugan);
    sprites = (GfxSprite **)self->base.data;
    sprites[0]->flags |= 1;
    sprites[0]->layerMask = 2;
    sprites[0]->alpha = sprites[6]->alpha;
    sprites[0]->posX = sprites[6]->posX + self->namePanelOffset[0] * sprites[6]->scaleX;
    sprites[0]->posY = sprites[6]->posY + self->namePanelOffset[1] * sprites[6]->scaleY;
    sprites[0]->flags |= 0x20;
    UiSpriteSetScaleRotation(sprites[0], sprites[6]->scaleX, sprites[6]->scaleY, 0.0f);

    UiBakuganSetAttributeCell(((GfxSprite **)self->base.data)[7],
                              self->entries[self->current].bakugan);
    sprites = (GfxSprite **)self->base.data;
    sprites[7]->flags |= 1;
    sprites[7]->layerMask = 2;
    sprites[7]->alpha = sprites[6]->alpha;
    sprites[7]->posX = sprites[6]->posX + self->namePanelOffset[2] * sprites[6]->scaleX;
    sprites[7]->posY = sprites[6]->posY + self->namePanelOffset[3] * sprites[6]->scaleY;
    sprites[7]->flags |= 0x20;
    UiSpriteSetScaleRotation(sprites[7], sprites[6]->scaleX, sprites[6]->scaleY, 0.0f);
  }
  else {
    UiTweenBegin(sprites[6]->scaleX, hide, sprite, &self->tweens[6], 1);
  }
}
