// bdc 0x088d3f70 GameStageGetLayoutTable
#include "bdc.h"

/* Returns the `{count, records}` pair of layout table `kind` (`g_gameStageLayoutTables``[kind]`) for the current
   stage `g_gameStageIndex`. */

s32 *GameStageGetLayoutTable(s32 kind)
{
  return g_gameStageLayoutTables[kind] + g_gameStageIndex * 2;
}
