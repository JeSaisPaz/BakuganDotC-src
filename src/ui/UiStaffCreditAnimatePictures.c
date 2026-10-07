// bdc 0x0894517c UiStaffCreditAnimatePictures
#include "bdc.h"

/* Animates the 13 pictures shown beside the roll of the staff-credits screen (task 3004,
   `UiStaffCreditCtor`): first reveals the next picture (`UiStaffCreditRevealNextPicture`), then
   steps each picture by its state `pictures[i].state` (1 = slide in by 4 px/frame, even pictures to
   the right and odd ones to the left, and fade in by 1/8; 2 = hold; 3 = grow 1.25 % per frame and
   fade out by 1/8, then hide; 0 = hidden). States switch when the roll frame `rollFrame` reaches
   frames derived from `g_staffCreditLayout` `picturesStart − pictureDelay` plus
   `pictureInterval` steps. */

void UiStaffCreditAnimatePictures(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  UiStaffCreditData *data;
  GfxSprite *sprite;
  s32 base;
  s32 state;
  s32 i;
  float w;
  float w2;
  float h;
  float h2;

  UiStaffCreditRevealNextPicture(screen);
  base = g_staffCreditLayout.picturesStart - credit->pictureDelay + 3;
  i = 0;
  do {
    state = credit->pictures[i].state;
    if (state == 1) {
      data = (UiStaffCreditData *)credit->base.data;
      if ((i & 1) != 0) {
        data->pictures[i]->posX = data->pictures[i]->posX - 4.0f;
      } else {
        data->pictures[i]->posX = data->pictures[i]->posX + 4.0f;
      }
      sprite = ((UiStaffCreditData *)credit->base.data)->pictures[i];
      sprite->alpha = sprite->alpha + 0.125f;
      if (!(credit->rollFrame < base + 8 + i * g_staffCreditLayout.pictureInterval)) {
        credit->pictures[i].state = 2;
      }
    } else if (state == 2) {
      if (!(credit->rollFrame < base - 8 + g_staffCreditLayout.pictureInterval * (i + 1))) {
        credit->pictures[i].state = 3;
      }
    } else if (state == 3) {
      w = GfxSpriteGetWidth(((UiStaffCreditData *)credit->base.data)->pictures[i]);
      w2 = GfxSpriteGetWidth(((UiStaffCreditData *)credit->base.data)->pictures[i]);
      h = GfxSpriteGetHeight(((UiStaffCreditData *)credit->base.data)->pictures[i]);
      sprite = ((UiStaffCreditData *)credit->base.data)->pictures[i];
      h2 = GfxSpriteGetHeight(sprite);
      UiSpriteSetSize(w + w2 * 0.1f * 0.125f, h + h2 * 0.1f * 0.125f, sprite);
      sprite = ((UiStaffCreditData *)credit->base.data)->pictures[i];
      sprite->alpha = sprite->alpha - 0.125f;
      if (!(credit->rollFrame < base + g_staffCreditLayout.pictureInterval * (i + 1))) {
        credit->pictures[i].state = 0;
        sprite = ((UiStaffCreditData *)credit->base.data)->pictures[i];
        sprite->flags = sprite->flags & ~1u;
      }
    }
    i++;
  } while (i < 13);
}
