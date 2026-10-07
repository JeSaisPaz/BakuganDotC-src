// bdc 0x08810ed8 ScriptOpPickLoadingTip
#include "bdc.h"

/* Script opcode that picks the tip of the next now-loading screen: operands u32 `mode`, u32 `col`;
   stores `UiLoadingPickTip``(col)` (mode 0, values 0 or 9..16 = `tips_001..008`) or
   `UiLoadingPickTipT``(col)` (mode 1, values 0..8 = `tips_t_001..008`) into script global 13
   (`*0x08ac58c4 + 0x34`), which `UiLoadingCtor` reads as its tip index; other modes store 0 (no
   tip). Returns 0. */

int ScriptOpPickLoadingTip(Script *script)

{
  u32 mode;
  u32 col;
  int tip;
  
  mode = ScriptReadU32(script);
  col = ScriptReadU32(script);
  tip = 0;
  if ((int)mode < 1) {
    if (-1 < (int)mode) {
      tip = UiLoadingPickTip(col);
    }
  }
  else if ((int)mode < 2) {
    tip = UiLoadingPickTipT(col);
  }
  g_scriptGlobalVars[0xd] = tip;
  return 0;
}

