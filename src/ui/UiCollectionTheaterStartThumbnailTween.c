// bdc 0x0898918c UiCollectionTheaterStartThumbnailTween
#include "bdc.h"

/* Starts the tweens of the six scene thumbnails (sprites 13..18 of `base.data`, `thumbTweens`) of
   `UiCollectionTheater` (task 315, `maybe_UiScreen315Ctor`). Fading in
   (`out == 0`): for each cell of the current page, a cell holding an unlocked scene
   (`UiCollectionTheaterIsCellUsed` and `sceneId != 0xff`) is made visible and set to its
   `"cinema_%02d"` thumbnail (`UiCollectionTheaterSetThumbnail`), any other cell is hidden; the
   sprite is moved back to its saved position (`thumbPos`, `thumbZ`) and
   `UiTweenBegin``(1.5, 0, sprite, tween, 3)` is started. Fading out: only
   `UiTweenBegin``(1.5, out, sprite, tween, 3)` for each thumbnail. */

void UiCollectionTheaterStartThumbnailTween(UiScreen *screen, u8 out)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite *sprite;
  s8 page;
  int i;

  if (out == 0) {
    for (i = 0; i < 6; i++) {
      page = self->page;
      if (UiCollectionTheaterIsCellUsed(screen, (u8)i, (u8)page) == 1) {
        sprite = ((GfxSprite **)self->base.data)[13 + i];
        if (self->sceneId[i + page * 6] == 0xff) {
          sprite->flags &= ~1u;
        } else {
          sprite->flags |= 1;
          UiCollectionTheaterSetThumbnail(screen, ((GfxSprite **)self->base.data)[13 + i],
                                          self->sceneId[i + self->page * 6]);
        }
      } else {
        sprite = ((GfxSprite **)self->base.data)[13 + i];
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[13 + i]->posX = self->thumbPos[i][0];
      ((GfxSprite **)self->base.data)[13 + i]->posY = self->thumbPos[i][1];
      ((GfxSprite **)self->base.data)[13 + i]->posZ = self->thumbZ[i];
      UiTweenBegin(1.5f, 0, ((GfxSprite **)self->base.data)[13 + i], &self->thumbTweens[i], 3);
    }
  } else {
    for (i = 0; i < 6; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[13 + i], &self->thumbTweens[i], 3);
    }
  }
}
