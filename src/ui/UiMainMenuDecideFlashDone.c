// bdc 0x089a8dc0 UiMainMenuDecideFlashDone
#include "bdc.h"

/* Steps flash slot 0 (`UiFlashStep`) and returns 1 when the decide flash is over. */

int UiMainMenuDecideFlashDone(UiMainMenu *self)
{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
