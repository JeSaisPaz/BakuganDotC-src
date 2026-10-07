// bdc 0x0890be9c UiLoadingStateInit
#include "bdc.h"

/* State 0 of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, vtable `0x08af47dc`,
   `UiLoadingCtor`; shared UI objects `0x08ac0e80`): switches to state 1 (`UiLoadingStateSetup`)
   with step 0. */

void UiLoadingStateInit(UiLoading *self)

{
  self->state = 1;
  self->step = 0;
  return;
}

