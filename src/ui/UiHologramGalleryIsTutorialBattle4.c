// bdc 0x08922144 UiHologramGalleryIsTutorialBattle4
#include "bdc.h"

/* Returns 1 during the tutorial (profile word 0x2b == 2, word 0x2e == 0) in battle 4, else 0. */

int UiHologramGalleryIsTutorialBattle4(void)

{
  SaveProfile *profile;
  u32 word;
  
  profile = SaveGetProfile();
  word = SaveProfileGetWord(profile,0x2b);
  if (word == 2) {
    profile = SaveGetProfile();
    word = SaveProfileGetWord(profile,0x2e);
    if ((word == 0) && (g_scriptGlobalVars[1] == 4)) {
      return 1;
    }
  }
  return 0;
}

