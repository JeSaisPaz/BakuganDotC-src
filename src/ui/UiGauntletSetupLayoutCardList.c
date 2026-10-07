// bdc 0x08933b20 UiGauntletSetupLayoutCardList
#include "bdc.h"

/* Positions the card-list sprites of the gauntlet setup screen (task 373,
   `maybe_UiScreen373Ctor`, class prefix `UiGauntletSetup`; card sprites `cc_card_L_%03d`,
   `c_set_OK_bo_*`, texts `DWCardName`/`DWCardHelp`; main update `UiGauntletSetupMainPhase`; focus area `+0x74`,
   item `+0x76`) relative to the list anchor sprite (sprite 7): sprites 8, 9, 48, 49, 40 and 41 each
   get x/y = anchor position + their offset (`slotOffsets[0..11]`) times the anchor's current scale
   (`scaleX`/`scaleY`), so the list follows the anchor's tween. The sprite table is re-read from
   `base.data` for every coordinate. */

void UiGauntletSetupLayoutCardList(UiGauntletSetup *self)
{
  GfxSprite *anchor;

  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[8]->posX = anchor->posX + self->slotOffsets[0] * anchor->scaleX;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[8]->posY = anchor->posY + self->slotOffsets[1] * anchor->scaleY;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[9]->posX = anchor->posX + self->slotOffsets[2] * anchor->scaleX;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[9]->posY = anchor->posY + self->slotOffsets[3] * anchor->scaleY;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[48]->posX = anchor->posX + self->slotOffsets[4] * anchor->scaleX;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[48]->posY = anchor->posY + self->slotOffsets[5] * anchor->scaleY;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[49]->posX = anchor->posX + self->slotOffsets[6] * anchor->scaleX;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[49]->posY = anchor->posY + self->slotOffsets[7] * anchor->scaleY;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[40]->posX = anchor->posX + self->slotOffsets[8] * anchor->scaleX;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[40]->posY = anchor->posY + self->slotOffsets[9] * anchor->scaleY;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[41]->posX = anchor->posX + self->slotOffsets[10] * anchor->scaleX;
  anchor = ((GfxSprite **)self->base.data)[7];
  ((GfxSprite **)self->base.data)[41]->posY = anchor->posY + self->slotOffsets[11] * anchor->scaleY;
}
