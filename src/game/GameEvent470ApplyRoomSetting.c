// bdc 0x088efc54 GameEvent470ApplyRoomSetting
#include "bdc.h"

/* Reads the per-room word `0x08b00cdc[area*5 + room]`; when bit 0 is set passes `value >> 1` to the
   field task (task 500, `GameFieldEmptyHookB`, an empty function in this build).
   UB (original binary): the table has 30 words (areas 0..5) and the index is not bounded; areas 6
   and 7 read the next fields of `g_gameEventState`, higher areas (the hub, area 8, reads index 40)
   read past the struct into the bss after it. Indices past the struct read as 0 here (bit 0 clear:
   the hook, empty anyway, is not called). */

void GameEvent470ApplyRoomSetting(void)

{
  u32 index = (u32)g_gameEventState.flags[0] * 5 + g_gameEventState.flags[2];
  u16 word = 0;

  if (__builtin_offsetof(GameEventState, roomSettings) + (index + 1) * sizeof(u16) <=
      sizeof(GameEventState)) {
    word = *(u16 *)(&g_gameEventState.roomSettings[index]);
  }
  if ((word & 1) != 0) {
    CoreTaskFind(500);
    GameFieldEmptyHookB();
  }
  return;
}
