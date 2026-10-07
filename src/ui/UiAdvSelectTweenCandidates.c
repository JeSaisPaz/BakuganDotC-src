// bdc 0x0891a914 UiAdvSelectTweenCandidates
#include "bdc.h"

/* Starts the open/close slide tweens of the candidate row of the adventure partner-select screen.
   Three sprite groups (sprite list `base.data`, tween `tweens[i]` per sprite i) slide along X:
   sprites 5..10 relative to `spritePos[5]`, sprites 11..16 (character portraits) relative to
   `spritePos[11]`, sprites 18..23 relative to `spritePos[5] + candidateOffset`.
   Show (`hide == 0`): each sprite is first placed at its group origin, made visible (portraits and
   18..23 only if the candidate slot is filled / locked; locked portraits are tinted 0.5 grey) and
   slides out to its recorded position. Hide: slides back from its position toward the origin with
   fade-out. */

void UiAdvSelectTweenCandidates(UiAdvSelect *self, u8 hide)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  UiAdvSelectCandidate *cand;
  float originX;
  int i;

  if (hide == 0) {
    for (i = 5; i < 11; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      originX = self->spritePos[5][0];
      ((GfxSprite **)self->base.data)[i]->posX = originX;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - originX, hide,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 11; i < 17; i++) {
      cand = &self->candidates[i - 11];
      sprite = ((GfxSprite **)self->base.data)[i];
      if (cand->bakugan != 0) {
        sprite->flags |= 1;
        UiAdvSelectSetCharaPicture(self, ((GfxSprite **)self->base.data)[i], cand->bakugan);
        sprite = ((GfxSprite **)self->base.data)[i];
        if (cand->locked != 0) {
          sprite->tint[0] = 0.5f;
          sprite->tint[1] = 0.5f;
          sprite->tint[2] = 0.5f;
          sprite->alpha = 0.0f;
          sprite = ((GfxSprite **)self->base.data)[i];
        }
      } else {
        sprite->flags &= ~1u;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      originX = self->spritePos[11][0];
      sprite->posX = originX;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][0] - originX, hide,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 18; i < 24; i++) {
      cand = &self->candidates[i - 18];
      sprite = ((GfxSprite **)self->base.data)[i];
      if (cand->locked != 0) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->posX = self->spritePos[5][0] + self->candidateOffsetX;
      ((GfxSprite **)self->base.data)[i]->posY = self->spritePos[5][1] + self->candidateOffsetY;
      UiTweenBeginSlide(1.0f, 0.0f,
                        (self->spritePos[i - 13][0] + self->candidateOffsetX) -
                            (self->spritePos[5][0] + self->candidateOffsetX),
                        hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
  } else {
    for (i = 5; i < 11; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[5][0] - self->spritePos[i][0], hide,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 11; i < 17; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[11][0] - self->spritePos[i][0], hide,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
    for (i = 18; i < 24; i++) {
      UiTweenBeginSlide(1.0f, 0.0f,
                        (self->spritePos[5][0] + self->candidateOffsetX) -
                            (self->spritePos[i - 13][0] + self->candidateOffsetX),
                        hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
    }
  }
}
