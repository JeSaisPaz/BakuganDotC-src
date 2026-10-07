// bdc 0x089a531c UiPulseReset
#include "bdc.h"

/* Clears a 0x28-byte cursor pulse record (see `UiPulseInit`). */

void UiPulseReset(UiPulse *self)

{
  memset(self,0,0x28);
  return;
}

