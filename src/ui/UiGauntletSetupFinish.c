// bdc 0x08932f84 UiGauntletSetupFinish
#include "bdc.h"

/* Final step of `UiGauntletSetupMainPhase`: reports the menu result through `UiSetMenuResult`
   — 1 when the screen was left normally (`+0xcad` = 0), 0 when the cancel flag `+0xcad` is set.
    */

void UiGauntletSetupFinish(UiGauntletSetup *self)

{
  if (self->cancelled == '\0') {
    UiSetMenuResult(&self->base,1);
    return;
  }
  UiSetMenuResult(&self->base,0);
  return;
}

