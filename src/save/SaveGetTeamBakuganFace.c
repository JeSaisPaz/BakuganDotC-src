// bdc 0x0880ddc8 SaveGetTeamBakuganFace
#include "bdc.h"

/* Returns the talk-face id of the Bakugan in team slot `slot` (-1 = active slot, see
   `SaveGetTeamBakugan`): 0x23..0x2e by species (ids 1/2/16 → 0x23, 3/4 → 0x24, 5/17 → 0x25,
   6/18 → 0x26, 7/19 → 0x27, 8/20 → 0x28, 9/10 → 0x29, 11..15 → 0x2a..0x2e, any other id → 0x23),
   or 0x2f when `ScriptVarsModeIs1`. */

s32 SaveGetTeamBakuganFace(s32 slot)
{
  u32 species;

  if (slot == -1) {
    slot = SaveProfileGetWord(SaveGetProfile(), 0x13);
  }
  species = SaveProfileGetWord(SaveGetProfile(), slot + 3);
  if (ScriptVarsModeIs1()) {
    return 0x2f;
  }
  switch (species) {
  case 1:
  case 2:
  case 16:
    return 0x23;
  case 3:
  case 4:
    return 0x24;
  case 5:
  case 17:
    return 0x25;
  case 6:
  case 18:
    return 0x26;
  case 7:
  case 19:
    return 0x27;
  case 8:
  case 20:
    return 0x28;
  case 9:
  case 10:
    return 0x29;
  case 11:
    return 0x2a;
  case 12:
    return 0x2b;
  case 13:
    return 0x2c;
  case 14:
    return 0x2d;
  case 15:
    return 0x2e;
  default:
    return 0x23;
  }
}
