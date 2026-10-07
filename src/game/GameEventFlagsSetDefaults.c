// bdc 0x088f5398 GameEventFlagsSetDefaults
#include "bdc.h"

/* Sets the 7 default story flags listed at `0x08a993d8` (`GameEventFlagSet`). Called by
   `GameEventStateClear`. */

void GameEventFlagsSetDefaults(void)

{
  u8 i;

  for (i = 0; i < 7; i++) {
    GameEventFlagSet(g_gameEventDefaultFlags[i]);
  }
}
