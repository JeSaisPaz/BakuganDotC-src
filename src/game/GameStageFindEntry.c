// bdc 0x088d3e2c GameStageFindEntry
#include "bdc.h"

/* Returns the entry of the stage table (`g_gameStageTable`, 41 `GameStageEntry` records; current
   stage `g_gameStageIndex`) for `stage` (−1 = current stage), or NULL. */

const GameStageEntry *GameStageFindEntry(s32 stage)

{
  const GameStageEntry *entry;
  s32 i;

  entry = g_gameStageTable;
  if (stage == -1) {
    stage = g_gameStageIndex;
  }
  i = 0;
  while (i < GameStageGetCount()) {
    i++;
    if (entry->stage == stage) {
      return entry;
    }
    entry++;
  }
  return NULL;
}
