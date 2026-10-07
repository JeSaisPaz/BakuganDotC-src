// bdc 0x0880d2a0 SaveHasProfile
#include "bdc.h"

/* Returns whether the player profile object exists (`g_playerProfile != NULL`); the guard callers
   use before `SaveGetProfile`. */

bool SaveHasProfile(void)

{
  return g_playerProfile != 0;
}

