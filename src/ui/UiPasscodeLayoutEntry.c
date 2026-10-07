// bdc 0x0893e7a8 UiPasscodeLayoutEntry
#include "bdc.h"

/* Lays out the entered symbols of `UiPasscode`: for each of the 6 entry slots
   (sprites 0x0e–0x13) shows the first `entryCount` ones centred on sprite 0x0d (18 px apart) with
   the symbol cells from `entry` (5-column sheet), hides the rest, and, when the entry is not
   complete (`entryCount` ≠ `answerLen`), shows the next slot as the input marker (cell 0,5). */

void UiPasscodeLayoutEntry(UiScreen *screen)
{
  UiPasscode *pc = (UiPasscode *)screen;
  GfxSprite **sprites;
  GfxSprite *spr;
  u32 count;
  s32 x;
  s32 i;

  count = pc->entryCount;
  x = (s32)count * -9;
  for (i = 0; i < 6; i++) {
    sprites = (GfxSprite **)screen->data;
    spr = sprites[0x0e + i];
    if (i < (s32)count) {
      spr->flags |= 1;
      sprites = (GfxSprite **)screen->data;
      sprites[0x0e + i]->posX = sprites[0x0d]->posX + (float)x;
      sprites = (GfxSprite **)screen->data;
      sprites[0x0e + i]->posY = sprites[0x0d]->posY;
      sprites = (GfxSprite **)screen->data;
      GfxSpriteSetCell(sprites[0x0e + i], (float)(pc->entry[i] / 5), (float)(pc->entry[i] % 5));
      x += 18;
    } else {
      spr->flags &= ~1u;
    }
    count = pc->entryCount;
  }
  if (count != pc->answerLen) {
    sprites = (GfxSprite **)screen->data;
    sprites[0x0e + count]->flags |= 1;
    sprites = (GfxSprite **)screen->data;
    sprites[0x0e + pc->entryCount]->posX = sprites[0x0d]->posX + (float)x;
    sprites = (GfxSprite **)screen->data;
    sprites[0x0e + pc->entryCount]->posY = sprites[0x0d]->posY;
    sprites = (GfxSprite **)screen->data;
    GfxSpriteSetCell(sprites[0x0e + pc->entryCount], 0.0f, 5.0f);
  }
}
