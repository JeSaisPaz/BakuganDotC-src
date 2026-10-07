// bdc 0x0894b1f8 UiBattleRecordSetTotalCell
#include "bdc.h"

/* Writes the usage share of one Bakugan in the totals table of the battle-record screen (task 3005,
   `UiBattleRecordCtor`): percent = battles[bakugan] * 100 / sum of battles over all 20 Bakugan,
   rounded up (at least 1 when non-zero, 0 when the Bakugan has no battles). The percentage is drawn
   with up to three digit sprites (layout sprite list `base.data`, ones digit at index `row*4 + 47`,
   tens at `-1`, hundreds at `-2`, each positioned relative to the hundreds slot) and a bar sprite
   (index `75 + row`) whose width and UV width are `percent * 154 / 100`. */

void UiBattleRecordSetTotalCell(UiBattleRecord *self, s32 row, s32 bakugan)
{
  s32 digit = row * 4 + 47;
  s32 bar = 75 + row;
  s32 count = self->battles[bakugan];
  s32 percent = 0;
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[digit];
  GfxSprite *barSprite;
  float width;
  float rect[4];
  s32 i;
  s32 total;
  s32 tens;

  if (count != 0) {
    total = 0;
    for (i = 0; i < 20; i++) {
      total += self->battles[i];
    }
    percent = (count * 100) / total;
    if (!(((float)count * 100.0f) / (float)total <= (float)percent)) {
      percent++;
    } else if (percent == 0) {
      percent = 1;
    }
  }

  if (percent < 10) {
    GfxSpriteSetCell(sprite, (float)(percent / 5), (float)(percent % 5));
    ((GfxSprite **)self->base.data)[digit]->posX =
        ((GfxSprite **)self->base.data)[digit - 2]->posX + 8.0f;
    ((GfxSprite **)self->base.data)[digit]->flags |= 1;
    ((GfxSprite **)self->base.data)[digit - 1]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[digit - 2]->flags &= ~1u;
  } else if (percent < 100) {
    GfxSpriteSetCell(sprite, (float)((percent % 10) / 5), (float)((percent % 10) % 5));
    ((GfxSprite **)self->base.data)[digit]->posX =
        ((GfxSprite **)self->base.data)[digit - 2]->posX + 12.0f;
    tens = percent / 10;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[digit - 1], (float)(tens / 5),
                     (float)(tens % 5));
    ((GfxSprite **)self->base.data)[digit - 1]->posX =
        ((GfxSprite **)self->base.data)[digit - 2]->posX + 4.0f;
    ((GfxSprite **)self->base.data)[digit]->flags |= 1;
    ((GfxSprite **)self->base.data)[digit - 1]->flags |= 1;
    ((GfxSprite **)self->base.data)[digit - 2]->flags &= ~1u;
  } else {
    GfxSpriteSetCell(sprite, (float)((percent % 10) / 5), (float)((percent % 10) % 5));
    ((GfxSprite **)self->base.data)[digit]->posX =
        ((GfxSprite **)self->base.data)[digit - 2]->posX + 16.0f;
    tens = (percent % 100) / 10;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[digit - 1], (float)(tens / 5),
                     (float)(tens % 5));
    ((GfxSprite **)self->base.data)[digit - 1]->posX =
        ((GfxSprite **)self->base.data)[digit - 2]->posX + 8.0f;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[digit - 2], (float)((percent / 100) / 5),
                     (float)((percent / 100) % 5));
    ((GfxSprite **)self->base.data)[digit]->flags |= 1;
    ((GfxSprite **)self->base.data)[digit - 1]->flags |= 1;
    ((GfxSprite **)self->base.data)[digit - 2]->flags |= 1;
  }

  barSprite = ((GfxSprite **)self->base.data)[bar];
  width = (float)percent * 154.0f * 0.01f;
  UiSpriteSetSize(154.0f, GfxSpriteGetHeight(barSprite), barSprite);
  ((GfxSprite **)self->base.data)[bar]->posX = 189.0f;

  barSprite = ((GfxSprite **)self->base.data)[bar];
  rect[3] = GfxSpriteGetHeight(barSprite);
  rect[0] = 4.0f;
  rect[1] = 0.0f;
  rect[2] = width;
  GfxSpriteSetUvRectXYWH(barSprite, rect);

  barSprite = ((GfxSprite **)self->base.data)[bar];
  UiSpriteSetSize(width, GfxSpriteGetHeight(barSprite), barSprite);
}
