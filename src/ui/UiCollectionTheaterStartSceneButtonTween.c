// bdc 0x0898a058 UiCollectionTheaterStartSceneButtonTween
#include "bdc.h"

/* Starts the zoom tweens of the scene buttons of `UiCollectionTheater`
   (task 315, `maybe_UiScreen315Ctor`): data sprites 0..5 (cell frames, `sceneButtonTweens`),
   31..36 (scene labels, `sceneTweensD`), 7..12 (`sceneTweensB`) and 19..24 (`sceneTweensC`).
   Fading in (`out == 0`): for each cell of the current page (`UiCollectionTheaterIsCellUsed`),
   the frame and label are shown for a used cell and hidden otherwise; the frame is reset to
   `"waku_4_b"` (`UiCollectionTheaterSetCellFrame`) with its add colour cleared, the label set
   from `sceneId` (`UiCollectionTheaterSetSceneLabel`); sprite 7+i is shown only for a used cell
   whose `sceneId` is 0xff (no scene), sprite 19+i only for a used cell whose `sceneNew` flag is
   set. Every sprite gets its saved z back and `UiTweenBegin``(1.5, 0, sprite, tween, 3)`.
   Fading out: only `UiTweenBegin``(1.5, out, sprite, tween, 3)` for each of them. */

void UiCollectionTheaterStartSceneButtonTween(UiScreen *screen, u8 out)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite *sprite;
  s8 page;
  int i;

  if (out == 0) {
    for (i = 0; i < 6; i++) {
      if (UiCollectionTheaterIsCellUsed(screen, (u8)i, (u8)self->page) == 1) {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags |= 1;
      } else {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags &= ~1u;
      }
      UiCollectionTheaterSetCellFrame(screen, ((GfxSprite **)self->base.data)[i], 0);
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 0.0f;
      UiTweenBegin(1.5f, 0, ((GfxSprite **)self->base.data)[i], &self->sceneButtonTweens[i], 3);
    }
    for (i = 31; i < 37; i++) {
      if (UiCollectionTheaterIsCellUsed(screen, (u8)(i - 31), (u8)self->page) == 1) {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags |= 1;
      } else {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags &= ~1u;
      }
      UiCollectionTheaterSetSceneLabel(screen, ((GfxSprite **)self->base.data)[i],
                                       self->sceneId[(i - 31) + self->page * 6]);
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZTail[i - 19];
      UiTweenBegin(1.5f, 0, ((GfxSprite **)self->base.data)[i], &self->sceneTweensD[i - 31], 3);
    }
    for (i = 7; i < 13; i++) {
      page = self->page;
      if (UiCollectionTheaterIsCellUsed(screen, (u8)(i - 7), (u8)page) == 1) {
        sprite = ((GfxSprite **)self->base.data)[i];
        if (self->sceneId[(i - 7) + page * 6] == 0xff) {
          sprite->flags |= 1;
        } else {
          sprite->flags &= ~1u;
        }
      } else {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
      UiTweenBegin(1.5f, 0, ((GfxSprite **)self->base.data)[i], &self->sceneTweensB[i - 7], 3);
    }
    for (i = 19; i < 25; i++) {
      page = self->page;
      if (UiCollectionTheaterIsCellUsed(screen, (u8)(i - 19), (u8)page) == 1) {
        sprite = ((GfxSprite **)self->base.data)[i];
        if (self->sceneNew[(i - 19) + page * 6] != 0) {
          sprite->flags |= 1;
        } else {
          sprite->flags &= ~1u;
        }
      } else {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZTail[i - 19];
      UiTweenBegin(1.5f, 0, ((GfxSprite **)self->base.data)[i], &self->sceneTweensC[i - 19], 3);
    }
  } else {
    for (i = 0; i < 6; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->sceneButtonTweens[i], 3);
    }
    for (i = 31; i < 37; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->sceneTweensD[i - 31], 3);
    }
    for (i = 7; i < 13; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->sceneTweensB[i - 7], 3);
    }
    for (i = 19; i < 25; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->sceneTweensC[i - 19], 3);
    }
  }
}
