// bdc 0x089a8100 UiMainMenuRefreshFlagSprites
#include "bdc.h"

/* Shows layout sprites 15/17 (`UiMainMenuSetSprites15And17Visible`) when menu flag 1
   (`UiMenuFlagsTest`) is clear, hides them otherwise. Called every frame by
   `UiMainMenuPhaseNetConnect`. */

void UiMainMenuRefreshFlagSprites(UiMainMenu *self)
{
  if (UiMenuFlagsTest(1) == 0) {
    UiMainMenuSetSprites15And17Visible(self, 1);
  } else {
    UiMainMenuSetSprites15And17Visible(self, 0);
  }
}
