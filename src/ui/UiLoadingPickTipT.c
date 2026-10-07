// bdc 0x0890bc78 UiLoadingPickTipT
#include "bdc.h"

/* Picks a now-loading tip of the `tips_t_%03d.tm2` kind for `ScriptOpPickLoadingTip` mode 1: from
   the 8x2 table `0x08a9b278`, row from the per-stage map `0x08a9b1d8[globalVars[1]]` (clamped
   0..7), column `col` (clamped 0..1; `col == 2` picks the column at random with `vrndi.s`, lifted to `PlatformRandU32`). Entry
   -1 gives a random tip 1..8, -2 gives 0 (no tip), any other entry `v` gives `v + 1`. The result
   indexes the tip table `0x08a9b060` read by `UiLoadingCtor` (1..8 = `tips_t_001..008`). */

int UiLoadingPickTipT(int col)

{
  int row;
  int entry;

  row = g_loadingTipTRowMap[g_scriptGlobalVars[1]];
  if (row < 0) {
    row = 0;
  }
  else if (7 < row) {
    row = 7;
  }
  if (col == 2) {
    col = (PlatformRandU32() >> 16) * 2 >> 16;
  }
  if (col < 0) {
    col = 0;
  }
  else if (1 < col) {
    col = 1;
  }
  entry = g_loadingTipTTable[row][col];
  if (entry == -1) {
    return ((PlatformRandU32() >> 16) * 8 >> 16) + 1;
  }
  if (entry == -2) {
    return 0;
  }
  return entry + 1;
}
