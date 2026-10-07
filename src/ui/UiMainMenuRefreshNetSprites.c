// bdc 0x089a814c UiMainMenuRefreshNetSprites
#include "bdc.h"

/* In a net session (profile flag 0): hides the net sprites (`UiMainMenuSetNetSpritesVisible` 0)
   for player index 1 (the guest) and shows them for the host or an unknown index. */

void UiMainMenuRefreshNetSprites(UiMainMenu *self)
{
  s32 idx;

  if (SaveGetProfileFlag0() != 0) {
    idx = NetGetLocalPlayerIndex();
    if (idx == 0) {
      UiMainMenuSetNetSpritesVisible(self, 1);
    } else if (idx == 1) {
      UiMainMenuSetNetSpritesVisible(self, 0);
    } else {
      UiMainMenuSetNetSpritesVisible(self, 1);
    }
  }
}
