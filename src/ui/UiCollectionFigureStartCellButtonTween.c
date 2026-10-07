// bdc 0x0898fb74 UiCollectionFigureStartCellButtonTween
#include "bdc.h"

/* Starts the appear (`out == 0`) or disappear tween (scale 1.5, flags 3) of the grid cell
   sprites of the figure collection screen: cell frames 0..5, name labels 0x19..0x1e and
   the two per-cell overlay sets 7..12 and 13..18 (tweens `self->tweens[i]`). On appear it
   first refreshes them for the current page/category: each sprite is shown only if
   `UiCollectionFigureIsCellUsed`; frames get the unselected `"waku_4_b"` texture
   (`UiCollectionFigureSetCellFrame`), labels the entry's name
   (`UiCollectionFigureSetNameTexture`), overlays 7..12 are shown when the entry id is 0,
   overlays 13..18 when the entry's `entryNew` flag is set; frames and overlays get their
   depth from `spriteZ[i]` and a cleared add colour. */

void UiCollectionFigureStartCellButtonTween(UiCollectionFigure *self, u8 out)
{
  int i;
  int page;
  GfxSprite *sprite;

  if (out == 0) {
    for (i = 0; i < 6; i++) {
      if (UiCollectionFigureIsCellUsed(self, self->category, (u8)i, self->page) == 1) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      } else {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
      UiCollectionFigureSetCellFrame(self, ((GfxSprite **)self->base.data)[i], false);
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 0.0f;
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x19; i < 0x1f; i++) {
      if (UiCollectionFigureIsCellUsed(self, self->category, (u8)(i - 0x19), self->page) == 1) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      } else {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
      UiCollectionFigureSetNameTexture(self, ((GfxSprite **)self->base.data)[i],
                                       self->entryIds[self->page * 6 + (i - 0x19)]);
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 7; i < 0xd; i++) {
      page = self->page;
      if (UiCollectionFigureIsCellUsed(self, self->category, (u8)(i - 7), (u8)page) == 1 &&
          self->entryIds[page * 6 + (i - 7)] == 0) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      } else {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 0.0f;
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0xd; i < 0x13; i++) {
      page = self->page;
      if (UiCollectionFigureIsCellUsed(self, self->category, (u8)(i - 0xd), (u8)page) == 1 &&
          self->entryNew[page * 6 + (i - 0xd)] != 0) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      } else {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 0.0f;
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0; i < 6; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x19; i < 0x1f; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 7; i < 0xd; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0xd; i < 0x13; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
