// bdc 0x088efc54 GameEvent470ApplyRoomSetting
#include "bdc.h"

/* Reads the per-room word `0x08b00cdc[area*5 + room]`; when bit 0 is set passes `value >> 1` to the
   field task (task 500, `GameFieldEmptyHookB`, an empty function in this build). */

void GameEvent470ApplyRoomSetting(void)

{
  if ((*(ushort *)(&g_gameEventState.roomSettings[g_gameEventState.flags[0] * 5 + g_gameEventState.flags[2]]) & 1) != 0) {
    CoreTaskFind(500);
    GameFieldEmptyHookB();
  }
  return;
}

