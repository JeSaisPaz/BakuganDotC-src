// bdc 0x0893dbbc UiPasscodeSetResult
#include "bdc.h"

/* Reports the outcome of `UiPasscode`: `UiSetMenuResult` 0 when the judgement
   `+0x7dc` is 0, else 1. */

void UiPasscodeSetResult(UiPasscode *screen)

{
  if (screen->judgement == 0) {
    UiSetMenuResult(&screen->base,0);
    return;
  }
  UiSetMenuResult(&screen->base,1);
}
