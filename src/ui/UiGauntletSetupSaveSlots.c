// bdc 0x089359b4 UiGauntletSetupSaveSlots
#include "bdc.h"

/* Writes the slot state of `UiGauntletSetup` back to the save profile
   (`SaveGetProfile`): when the focused slot (`item`) is marked it clears its new-mark `slotNew`,
   hides sprite 0x22+slot and sets the card's seen bit in the profile bitset at `+0x531`
   (`ownedItems[0x17 + card / 8]`); then stores the two chosen cards `chosenCard[0..1]` (0xff when
   empty) at profile `equipSlots[curBakugan]`. */

void UiGauntletSetupSaveSlots(UiGauntletSetup *self)
{
  GfxSprite *sprite;
  SaveProfile *profile;
  int card;
  int cur;
  int i;

  if ((self->slotMark[self->item] & 1) != 0) {
    self->slotNew[self->item] = 0;
    sprite = ((GfxSprite **)self->base.data)[self->item + 0x22];
    sprite->flags &= ~1u;
  }
  if ((self->slotMark[self->item] & 1) != 0) {
    profile = SaveGetProfile();
    card = self->slotCard[self->item];
    profile->data->ownedItems[0x17 + card / 8] |= (u8)(1 << (card % 8));
  }
  for (i = 0; i < 2; i++) {
    if (self->chosenCard[i] == 0xff) {
      profile = SaveGetProfile();
      cur = SaveGetProfile()->data->curBakugan;
      profile->data->equipSlots[cur][i] = 0xff;
    }
    else {
      profile = SaveGetProfile();
      cur = SaveGetProfile()->data->curBakugan;
      profile->data->equipSlots[cur][i] = self->chosenCard[i];
    }
  }
}
