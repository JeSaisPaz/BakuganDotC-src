// bdc 0x089b1c14 SaveProfileSetMapMode
#include "bdc.h"

/* Stores the world-map mode in profile word 0x2b and its destination (a stage id) in word 0x2c
   (`SaveProfileSetWord`). */

void SaveProfileSetMapMode(u8 mode, u8 destination)
{
  SaveProfile *profile;

  profile = SaveGetProfile();
  SaveProfileSetWord(profile, 0x2b, (uint)mode);
  profile = SaveGetProfile();
  SaveProfileSetWord(profile, 0x2c, (uint)destination);
}
