// bdc 0x088d3f70 GameStageGetLayoutTable
#include "bdc.h"

/* Returns the `BtlStagePlacementList` `{count, records}` row of layout table `kind`
   (`g_gameStageLayoutTables``[kind]`) for the current stage `g_gameStageIndex`. */

BtlStagePlacementList *GameStageGetLayoutTable(s32 kind)
{
  return g_gameStageLayoutTables[kind] + g_gameStageIndex;
}
