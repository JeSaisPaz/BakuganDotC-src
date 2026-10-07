// bdc 0x088efc54 GameEvent470ApplyRoomSetting
#include "bdc.h"

/* Reads the per-room word `0x08b00cdc[area*5 + room]`; when bit 0 is set passes `value >> 1` to the
   field task (task 500, `GameFieldEmptyHookB`, an empty function in this build). */

void GameEvent470ApplyRoomSetting(void)

{
  if ((*(ushort *)((u8 *)g_gameEventFlags + sizeof(g_gameEventFlags) + g_gameEventFlags[0] * 10 + g_gameEventFlags[2] * 2) & 1) != 0) {
    CoreTaskFind(500);
    GameFieldEmptyHookB();
  }
  return;
}

