// bdc 0x08992370 UiUnlockCodeIsDimensionsMode
#include "bdc.h"

/* Returns whether bit 2 of the profile halfword `playthroughClearBits` is set; `UiUnlockCodeCtor`
   stores it in `dimensionsMode`, which selects the 10-digit (Bakugan Dimensions) code mode. */

bool UiUnlockCodeIsDimensionsMode(void)
{
  SaveProfile *profile;

  profile = SaveGetProfile();
  return (profile->data->playthroughClearBits & 4) != 0;
}
