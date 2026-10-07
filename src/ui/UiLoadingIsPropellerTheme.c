// bdc 0x0890afd8 UiLoadingIsPropellerTheme
#include "bdc.h"

/* Returns 1 when the theme `+0x18` of the now-loading screen (task 10100 / 0x2774, 0x240 bytes,
   vtable `0x08af47dc`, `UiLoadingCtor`; shared UI objects `0x08ac0e80`) is 0x11..0x13 (the themed
   frames drawn with propellers, see `UiLoadingDrawPropellers`), else 0. */

bool UiLoadingIsPropellerTheme(UiLoading *self)

{
  if ((0x10 < self->theme) && (self->theme < 0x14)) {
    return true;
  }
  return false;
}

