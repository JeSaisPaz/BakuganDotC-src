// bdc 0x0880df94 SaveGetTeamBakuganOrMode1
#include "bdc.h"

/* Returns the raw Bakugan id in team slot `slot` (-1 = active, profile word `slot + 3`), or 0x15
   when `ScriptVarsModeIs1` (a fixed special Bakugan in that mode). */

s32 SaveGetTeamBakuganOrMode1(s32 slot)

{
  SaveProfile *profile;
  u32 id;
  
  if (slot == -1) {
    profile = SaveGetProfile();
    slot = SaveProfileGetWord(profile,0x13);
  }
  profile = SaveGetProfile();
  id = SaveProfileGetWord(profile,slot + 3);
  if (ScriptVarsModeIs1()) {
    id = 0x15;
  }
  return id;
}

