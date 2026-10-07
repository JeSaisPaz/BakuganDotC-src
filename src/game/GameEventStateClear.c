// bdc 0x088eab38 GameEventStateClear
#include "bdc.h"

/* Clears the story event state block `0x08b00bb0` (0x17c bytes: area/room `0x08b00bd4`/`bd6`, the
   counter byte `0x08b00bd9` and the flag bitset `0x08b00bdc` used by `GameEventFlagTest`) and
   calls `GameEventFlagsSetDefaults`. Called by `GameFieldRestoreProgress`. */

void GameEventStateClear(void)

{
  memset(&g_gameEventState,0,0x17c);
  GameEventFlagsSetDefaults();
  return;
}

