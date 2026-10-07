// bdc 0x0890d0c8 UiLoadingStateWaitDisc
#include "bdc.h"

/* State 2 of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, vtable `0x08af47dc`,
   `UiLoadingCtor`; shared UI objects `g_uiLoadingShared`), sub-stepped by `step`:
   - step 0: becomes step 1 and runs step 1 in the same frame;
   - step 1: non-propeller themes show the tip (`UiLoadingShowTip`); while no tip picture
     (`hasTipImage` clear) and the disc manager exists, a busy disc re-arms `waitTimer` to 15,
     an idle one counts it down and then advances to step 2;
   - step 2: propeller themes set `propPhase` to 1, others show the tip; advances to step 3;
   - any other step: once the disc manager is idle (propeller themes also need `propTimer >= 6`),
     either goes back to step 1 with a 45-frame `waitTimer` (`noTip` clear) or resets `step` and
     advances `state`.
   Non-propeller themes then animate the icons (`UiLoadingAnimateIcons`) and sync the tip
   scroll (`UiLoadingSyncTipScroll`). */

void UiLoadingStateWaitDisc(UiLoading *self)
{
  switch (self->step) {
  case 0:
    self->step = self->step + 1;
    /* fall through */
  case 1:
    if (!UiLoadingIsPropellerTheme(self)) {
      UiLoadingShowTip(self);
    }
    if (self->hasTipImage == 0 && IoDiscHasManager()) {
      if (!IoDiscIsIdle(IoDiscGetManager())) {
        self->waitTimer = 15;
      } else if (self->waitTimer > 0) {
        self->waitTimer = self->waitTimer - 1;
      } else {
        self->step = self->step + 1;
      }
    }
    break;
  case 2:
    if (UiLoadingIsPropellerTheme(self)) {
      self->propPhase = 1;
    } else {
      UiLoadingShowTip(self);
    }
    self->step = self->step + 1;
    break;
  default:
    if (!UiLoadingIsPropellerTheme(self) || self->propTimer >= 6) {
      if (IoDiscIsIdle(IoDiscGetManager())) {
        if (self->noTip == 0) {
          self->waitTimer = 45;
          self->step = 1;
        } else {
          self->step = 0;
          self->state = self->state + 1;
        }
      }
    }
    break;
  }
  if (!UiLoadingIsPropellerTheme(self)) {
    UiLoadingAnimateIcons(self);
    UiLoadingSyncTipScroll(self);
  }
}
