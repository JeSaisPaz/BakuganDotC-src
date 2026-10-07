// bdc 0x0896b57c UiCardEquipSaveLoadout
#include "bdc.h"

/* Leaves `UiCardEquip`: unless the cancel flag `cancelled` is set, writes each
   Bakugan's up to two active cards (profile words `0x36 + i*2 + slot`, first cleared to 0xff, then
   filled with the card ids whose active bit is set in the flag bytes `cardIds[0x10 + i*4 + k]`) and
   its G-power gauge value (word `0x46 + i`) into the save profile (`SaveGetProfile`) and sets menu
   result 1; when cancelled sets result 0 (`UiSetMenuResult`). */

void UiCardEquipSaveLoadout(UiCardEquip *self)
{
  SaveProfile *profile;
  s32 i;
  s32 j;
  s32 k;
  u8 slot;

  if (self->cancelled != 0) {
    UiSetMenuResult(&self->base, 0);
    return;
  }
  for (i = 0; i < self->bakuganCount; i++) {
    for (j = 0; j < 2; j++) {
      profile = SaveGetProfile();
      if (profile->words != NULL) {
        profile->words[0x36 + i * 2 + j] = 0xff;
      }
    }
    slot = 0;
    for (k = 0; k < 4; k++) {
      if ((self->cardIds[0x10 + i * 4 + k] & 1) != 0 && slot < 2) {
        profile = SaveGetProfile();
        if (profile->words != NULL) {
          profile->words[0x36 + i * 2 + slot] = self->cardIds[i * 4 + k];
        }
        slot++;
      }
    }
    profile = SaveGetProfile();
    if (profile->words != NULL) {
      profile->words[0x46 + i] = self->gauge[i];
    }
  }
  UiSetMenuResult(&self->base, 1);
}
