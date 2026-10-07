// bdc 0x088f18f4 GameEventOpAESetRoomSetting
#include "bdc.h"

/* Handler of event opcode 0xae (`GameEvent470ExecCommand`): passes `arg` to the field task
   (`GameFieldEmptyHookB`) and stores it in the per-room word `0x08b00cdc[area*5 + room]` (bit 0 set, value
   in bits 1..). */

void GameEventOpAESetRoomSetting(GameEvent *self, u8 flag, s16 arg)

{
  u16 *word;

  CoreTaskFind(500);
  GameFieldEmptyHookB();
  word = (u16 *)((u8 *)g_gameEventFlags + sizeof(g_gameEventFlags) + g_gameEventFlags[0] * 10 + g_gameEventFlags[2] * 2);
  *word = *word | 1;
  *word = (*word & 0xffff0001) | (((u16)arg & 0x7fff) << 1);
}
