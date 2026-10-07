// bdc 0x0891bb84 UiHologramGalleryIsTutorialLocked
#include "bdc.h"

/* Returns 1 during the tutorial (profile word 0x2b == 2, word 0x2e == 0) in battle 4 before profile
   flag `+0x82` bit 0 is set, else 0. */

int UiHologramGalleryIsTutorialLocked(void)
{
  if (SaveProfileGetWord(SaveGetProfile(), 0x2b) == 2) {
    if (SaveProfileGetWord(SaveGetProfile(), 0x2e) == 0 && g_scriptGlobalVars[1] == 4 &&
        (SaveGetProfile()->data->viewSeenMask & 1) == 0) {
      return 1;
    }
  }
  return 0;
}
