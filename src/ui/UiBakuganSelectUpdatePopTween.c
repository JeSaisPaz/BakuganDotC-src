// bdc 0x08930d3c UiBakuganSelectUpdatePopTween
#include "bdc.h"

/* Advances the name-panel pop tween of sprite 6 of `UiBakuganSelect` while
   `namePanelOn` is set: popping in (`namePanelFadeOut` = 0) runs `UiTweenUpdate` (scale 1.2→1,
   16 frames, mode 7) and makes sprites 0 and 7 follow its alpha, scale and position
   (`namePanelOffset` scaled by sprite 6's scale); fading out runs mode 1 and copies only the alpha.
   Clears `namePanelOn` once the tween reports it has finished. */

void UiBakuganSelectUpdatePopTween(UiBakuganSelect *self)
{
  bool done;
  GfxSprite **sprites;

  if (self->namePanelOn == 0) {
    return;
  }
  if (self->namePanelFadeOut == 0) {
    done = UiTweenUpdate(1.2f, 1.0f, 16.0f, self->namePanelFadeOut,
                         ((GfxSprite **)self->base.data)[6], &self->tweens[6], 7);
    sprites = (GfxSprite **)self->base.data;
    sprites[0]->alpha = sprites[6]->alpha;
    sprites = (GfxSprite **)self->base.data;
    sprites[0]->posX = sprites[6]->posX + self->namePanelOffset[0] * sprites[6]->scaleX;
    sprites = (GfxSprite **)self->base.data;
    sprites[0]->posY = sprites[6]->posY + self->namePanelOffset[1] * sprites[6]->scaleY;
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[0], sprites[6]->scaleX, sprites[6]->scaleY, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[7]->alpha = sprites[6]->alpha;
    sprites = (GfxSprite **)self->base.data;
    sprites[7]->posX = sprites[6]->posX + self->namePanelOffset[2] * sprites[6]->scaleX;
    sprites = (GfxSprite **)self->base.data;
    sprites[7]->posY = sprites[6]->posY + self->namePanelOffset[3] * sprites[6]->scaleY;
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[7], sprites[6]->scaleX, sprites[6]->scaleY, 0.0f);
  } else {
    done = UiTweenUpdate(1.2f, 1.0f, 16.0f, self->namePanelFadeOut,
                         ((GfxSprite **)self->base.data)[6], &self->tweens[6], 1);
    sprites = (GfxSprite **)self->base.data;
    sprites[0]->alpha = sprites[6]->alpha;
    sprites = (GfxSprite **)self->base.data;
    sprites[7]->alpha = sprites[6]->alpha;
  }
  if (done) {
    self->namePanelOn = 0;
  }
}
