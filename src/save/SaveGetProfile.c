// bdc 0x0880cc08 SaveGetProfile
#include "bdc.h"

/* Returns `g_playerProfile`, the player profile/save-data holder object (US decomp:
   `Get_DAT_08AAC9E0`). */

SaveProfile *SaveGetProfile(void)

{
  return g_playerProfile;
}

