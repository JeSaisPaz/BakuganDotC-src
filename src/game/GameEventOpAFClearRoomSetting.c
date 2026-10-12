// bdc 0x088f1980 GameEventOpAFClearRoomSetting
#include "bdc.h"

/* Handler of event opcode 0xaf (`GameEvent470ExecCommand`): calls the field task's `GameFieldEmptyHookA`
   and clears the per-room word. */

void GameEventOpAFClearRoomSetting(GameEvent *self, u8 flag, s16 arg)

{
  CoreTask *task;
  ushort *roomWord;
  
  task = CoreTaskFind(500);
  GameFieldEmptyHookA(task);
  roomWord = (ushort *)(&g_gameEventState.roomSettings[g_gameEventState.flags[0] * 5 + g_gameEventState.flags[2]]);
  *roomWord = *roomWord & 0xfffe;
  *roomWord = *roomWord & 1;
  return;
}

