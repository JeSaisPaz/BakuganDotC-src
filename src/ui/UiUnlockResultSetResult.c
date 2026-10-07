// bdc 0x089389a8 UiUnlockResultSetResult
#include "bdc.h"

/* Reports menu result 0 for `UiUnlockResult` (`UiSetMenuResult``(screen,
   0)`). */

void UiUnlockResultSetResult(UiUnlockResult *self)

{
  UiSetMenuResult(&self->base,0);
  return;
}

