// bdc 0x089c7210 SndManagerFindFreeSlot
#include "bdc.h"

/* Index (0..31) of the first free command-queue slot of the `SndManager` (`cmdHandle[i] == 0`),
   or -1 when all 32 are in use. Callers hold the manager lock. */

s32 SndManagerFindFreeSlot(SndManager *mgr)

{
  s32 i;

  for (i = 0; i < 0x20; i++) {
    if (mgr->cmdHandle[i] == 0) {
      return i;
    }
  }
  return -1;
}
