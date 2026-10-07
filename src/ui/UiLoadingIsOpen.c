// bdc 0x0890acfc UiLoadingIsOpen
#include "bdc.h"

/* Returns whether the now-loading task (id 10100 / 0x2774) exists. */

bool UiLoadingIsOpen(void)

{
  s32 exists;
  
  exists = CoreTaskExists(0x2774);
  return exists != 0;
}

