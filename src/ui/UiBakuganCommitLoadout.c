// bdc 0x0892be10 UiBakuganCommitLoadout
#include "bdc.h"

/* Writes the current Bakugan (`data->curBakugan`, low byte) and its loadout into the save profile's
   word table, each write going through `SaveGetProfile` / `SaveProfileSetWord` or skipped when
   `words` is NULL: word 3 = Bakugan id, words 4..6 = -1, words 54..55 = its two equip slot bytes
   (`equipSlots[id]`), words 56..61 and 62..69 = 0xff, words 70..73 = 100. */

void UiBakuganCommitLoadout(void)

{
  u32 bakugan;
  u8 slot;
  SaveProfile *profile;
  s32 i;
  s32 j;

  bakugan = SaveGetProfile()->data->curBakugan & 0xff;
  SaveProfileSetWord(SaveGetProfile(), 3, bakugan);
  SaveProfileSetWord(SaveGetProfile(), 4, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 5, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 6, 0xffffffff);
  for (i = 0; i < 2; i++) {
    slot = SaveGetProfile()->data->equipSlots[bakugan][i];
    profile = SaveGetProfile();
    if (profile->words != NULL) {
      profile->words[0x36 + i] = slot;
    }
  }
  for (i = 1; i < 4; i++) {
    for (j = 0; j < 2; j++) {
      profile = SaveGetProfile();
      if (profile->words != NULL) {
        profile->words[0x36 + i * 2 + j] = 0xff;
      }
    }
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 2; j++) {
      profile = SaveGetProfile();
      if (profile->words != NULL) {
        profile->words[0x3e + i * 2 + j] = 0xff;
      }
    }
  }
  profile = SaveGetProfile();
  if (profile->words != NULL) {
    profile->words[0x46] = 100;
  }
  for (i = 1; i < 4; i++) {
    profile = SaveGetProfile();
    if (profile->words != NULL) {
      profile->words[0x46 + i] = 100;
    }
  }
}
