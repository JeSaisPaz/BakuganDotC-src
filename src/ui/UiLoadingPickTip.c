// bdc 0x0890bd78 UiLoadingPickTip
#include "bdc.h"

/* Picks a now-loading tip of the `tips_%03d.tm2` kind for `ScriptOpPickLoadingTip` mode 0: from
   the 8x2 table `g_loadingTipTable`, row from the per-stage map
   `g_loadingTipRowMap``[globalVars[1]]` (clamped 0..7), column `col` (clamped 0..1). Entry -1
   gives a random 11..16 (`vrndi.s`, lifted to `PlatformRandU32`), -2 a random 11..16 other than 14 (drawn with
   `CoreRandNext``(6)`; forced when script global var 8 is 2), -3 gives 0 (no tip), any other
   entry `v` gives `v + 9`. The result indexes the tip table `0x08a9b060` read by `UiLoadingCtor`
   (9..16 = `tips_001..008`, message `n+7`). */

int UiLoadingPickTip(int col)

{
  int row;
  int entry;
  int tip;

  row = g_loadingTipRowMap[g_scriptGlobalVars[1]];
  if (row < 0) {
    row = 0;
  }
  else if (7 < row) {
    row = 7;
  }
  if (col < 0) {
    col = 0;
  }
  else if (1 < col) {
    col = 1;
  }
  entry = g_loadingTipTable[row][col];
  if (g_scriptGlobalVars[8] == 2) {
    entry = -2;
  }
  if (entry == -1) {
    return (int)(((PlatformRandU32() >> 16) * 6) >> 16) + 11;
  }
  if (entry == -2) {
    do {
      tip = (int)CoreRandNext(6) + 11;
    } while (tip == 14);
    return tip;
  }
  if (entry == -3) {
    return 0;
  }
  return entry + 9;
}
