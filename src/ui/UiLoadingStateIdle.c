// bdc 0x0890d080 UiLoadingStateIdle
#include "bdc.h"

/* State 3 of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, vtable `0x08af47dc`,
   `UiLoadingCtor`; shared UI objects `0x08ac0e80`): for non-propeller themes animates the icons
   (`UiLoadingAnimateIcons`), refreshes the tip (`UiLoadingShowTip`,
   `UiLoadingSyncTipScroll`); resets the step `+0x14`. */

void UiLoadingStateIdle(UiLoading *self)

{
  if (!UiLoadingIsPropellerTheme(self)) {
    UiLoadingAnimateIcons(self);
    UiLoadingShowTip(self);
    UiLoadingSyncTipScroll(self);
  }
  self->step = 0;
  return;
}

