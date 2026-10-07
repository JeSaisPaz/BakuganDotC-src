// bdc 0x0891cdc8 UiHologramGalleryEnsureVoicePac
#include "bdc.h"

/* Starts loading voice pac 1 unless it is already loaded (`SaveProfileTestWord30Bits(1) != 1` →
   `SaveProfileModifyWord30Bits(1, 1)`); returns 1 when a load was started, else 0. */

int UiHologramGalleryEnsureVoicePac(void)

{
  if (SaveProfileTestWord30Bits(1) != 1) {
    SaveProfileModifyWord30Bits(1,1);
    return 1;
  }
  return 0;
}
