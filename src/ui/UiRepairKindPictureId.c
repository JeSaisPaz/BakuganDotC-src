// bdc 0x0890f9b8 UiRepairKindPictureId
#include "bdc.h"

/* Returns the `repair_card_%03d` picture number for repair kind `kind` of the card repair screen
   (`UiRepairSetupPhase`): 3 → 2, every other kind → 1. */

int UiRepairKindPictureId(void *screen, int kind)

{
  if (kind >= 3 && kind < 4) {
    return 2;
  }
  return 1;
}
