// bdc 0x0880dd74 SaveGetTeamBakugan
#include "bdc.h"

/* Returns the Bakugan id (1..20) in team slot `slot` (profile word `slot + 3`); `slot = -1` means
   the active slot (profile word 0x13). An empty slot (0) reads as 1. */

s32 SaveGetTeamBakugan(s32 slot)
{
  u32 id;

  if (slot == -1) {
    slot = SaveProfileGetWord(SaveGetProfile(), 0x13);
  }
  id = SaveProfileGetWord(SaveGetProfile(), slot + 3);
  if (id == 0) {
    id = 1;
  }
  return id;
}
