// bdc 0x089564a4 UiEquipResetBattleOptions
#include "bdc.h"

/* Resets the battle option words of the save profile to their defaults (`SaveProfileSetWord`):
   word 0x18 = 1, 0x19 = 200, 0x1a = 0, and 0x1b = 3 when the battle rule (word 7) is 0, else 0. */

void UiEquipResetBattleOptions(void)

{
  SaveProfile *profile;
  u32 rule;
  
  profile = SaveGetProfile();
  SaveProfileSetWord(profile,0x18,1);
  profile = SaveGetProfile();
  SaveProfileSetWord(profile,0x19,200);
  profile = SaveGetProfile();
  SaveProfileSetWord(profile,0x1a,0);
  profile = SaveGetProfile();
  rule = SaveProfileGetWord(profile,7);
  if (rule == 0) {
    profile = SaveGetProfile();
    SaveProfileSetWord(profile,0x1b,3);
    return;
  }
  profile = SaveGetProfile();
  SaveProfileSetWord(profile,0x1b,0);
  return;
}

