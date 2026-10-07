// bdc 0x0890d250 UiLoadingStateClose
#include "bdc.h"

/* State 4 of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, vtable `0x08af47dc`,
   `UiLoadingCtor`; shared UI objects `0x08ac0e80`): step 0 hides the 23 shared sprites (clears
   bit 0 of `+0xd0`); step 1 restores the display frame skip from `0x08ac0e80+0x38` and removes the
   task (`CoreTaskRemove`). */

void UiLoadingStateClose(UiLoading *self)

{
  int i;

  if (self->step < 1) {
    if (-1 < self->step) {
      for (i = 0; i < 0x17; i++) {
        g_uiLoadingShared->sprites[i]->flags &= 0xfffffffe;
      }
      self->step = self->step + 1;
      return;
    }
  }
  else if (self->step < 2) {
    self->step = self->step + 1;
    g_gfxDisplay->frameSkip = g_uiLoadingShared->frameSkip;
    CoreTaskRemove(&self->base, true);
  }
  return;
}
