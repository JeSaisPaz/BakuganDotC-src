// bdc 0x089703d4 UiOptionApply
#include "bdc.h"

/* Leaves `UiOption`: sets menu result 0 (`UiSetMenuResult`) and writes the option
   values back to the profile (`UiOptionSyncProfile` with `store` = 1). */

void UiOptionApply(UiOption *self)

{
  UiSetMenuResult(&self->base,0);
  UiOptionSyncProfile(self,true);
  return;
}

