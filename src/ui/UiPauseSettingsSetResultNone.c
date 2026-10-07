// bdc 0x089abf44 UiPauseSettingsSetResultNone
#include "bdc.h"

/* Sets the screen's menu result to 0 (`UiSetMenuResult`) before it closes. */

void UiPauseSettingsSetResultNone(UiPauseSettings *self)

{
  UiSetMenuResult(&self->base,0);
  return;
}

