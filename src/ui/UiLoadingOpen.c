// bdc 0x0890accc UiLoadingOpen
#include "bdc.h"

/* Opens the now-loading screen: creates task 10100 (0x2774) at priority 100 (`CoreTaskCreate`)
   unless it already exists. */

void UiLoadingOpen(void)

{
  s32 exists;
  
  exists = CoreTaskExists(0x2774);
  if (exists == 0) {
    CoreTaskCreate(0x2774,100);
  }
  return;
}

