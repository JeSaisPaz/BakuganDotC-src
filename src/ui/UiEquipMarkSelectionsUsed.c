// bdc 0x0895c7f4 UiEquipMarkSelectionsUsed
#include "bdc.h"

/* Records the final picks of the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) in
   the player profile (`SaveGetProfile`): for each player sets the "used" bit of its Bakugan
   (profile word 3+p, `SaveProfileGetWord`) in the bit field at profile `+0x5de`, and of each of
   its two chosen equipment entries (profile words `+0xd8[p*8 + i*4]`, skipped when 0xff) in the bit
   field at `+0x5e8`. */

void UiEquipMarkSelectionsUsed(UiEquip *self)
{
  s32 player;
  s32 i;

  for (player = 0; player < self->playerCount; player++) {
    SaveProfile *profile = SaveGetProfile();
    s32 bakugan = (s32)SaveProfileGetWord(SaveGetProfile(), player + 3);
    profile->data->newItemGroups[0xb + bakugan / 8] |= (u8)(1 << (bakugan % 8));

    for (i = 0; i < 2; i++) {
      SaveProfile *check = SaveGetProfile();
      s32 equip = 0;
      if (check->words != NULL) {
        equip = (s32)check->words[0x36 + player * 2 + i];
      }
      if (equip != 0xff) {
        SaveProfile *target = SaveGetProfile();
        SaveProfile *source = SaveGetProfile();
        equip = 0;
        if (source->words != NULL) {
          equip = (s32)source->words[0x36 + player * 2 + i];
        }
        target->data->newItemGroups[0x15 + equip / 8] |= (u8)(1 << (equip % 8));
      }
    }
  }
}
