// bdc 0x089333e4 UiGauntletSetupReleasePlayerModel
#include "bdc.h"

/* Releases the player-avatar model `+0x1af0` of `UiGauntletSetup`
   (`CoreObjectDeferDelete(model, 0)`) and clears the pointer. */

void UiGauntletSetupReleasePlayerModel(UiGauntletSetup *self)

{
  if (self->playerModel != (CoreObject *)0x0) {
    CoreObjectDeferDelete(self->playerModel,0);
    self->playerModel = (void *)0x0;
  }
  return;
}

