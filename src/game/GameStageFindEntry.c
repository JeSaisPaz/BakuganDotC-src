// bdc 0x088d3e2c GameStageFindEntry
#include "bdc.h"

/* Returns the entry of the stage table (`g_gameStageTable`, 41 entries of `{s32 stage, ?, char
   *pack}`; current stage `g_gameStageIndex`) for `stage` (−1 = current stage), or NULL. */

s32 *GameStageFindEntry(s32 stage)

{
  s32 *entry;
  s32 i;

  entry = g_gameStageTable;
  if (stage == -1) {
    stage = g_gameStageIndex;
  }
  i = 0;
  while (i < GameStageGetCount()) {
    i++;
    if (*entry == stage) {
      return entry;
    }
    entry += 3;
  }
  return (s32 *)0;
}
