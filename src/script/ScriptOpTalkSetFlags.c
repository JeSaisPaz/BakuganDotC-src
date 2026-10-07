// bdc 0x0881031c ScriptOpTalkSetFlags
#include "bdc.h"

/* Script opcode that edits the flag word `+0xba4` of the talk/HUD task (id 0x6e, `BtlHudCtor`):
   operands u16 `mode`, u32 `mask`; mode 0 sets the bits (`|= mask`), mode 1 clears them. Without
   the task, mode 2 returns 2 (wait and retry), other modes return 0. */

int ScriptOpTalkSetFlags(Script *script)
{
  s32 mode = (s32)ScriptReadU16(script);
  u32 mask = ScriptReadU32(script);
  BtlHud *hud = (BtlHud *)CoreTaskFind(0x6e);

  if (hud == NULL) {
    if (mode == 2) {
      return 2;
    }
    return 0;
  }
  if (mode < 1) {
    if (mode >= 0) {
      hud->talkFlags |= mask;
    }
  } else if (mode < 2) {
    hud->talkFlags &= ~mask;
  }
  return 0;
}
