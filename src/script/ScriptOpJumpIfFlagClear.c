// bdc 0x089ca944 ScriptOpJumpIfFlagClear
#include "bdc.h"

/* Conditional jump: jumps to the target if the flag bit is clear. */

int ScriptOpJumpIfFlagClear(Script *script)
{
  u32 bit = ScriptReadU16(script);
  u32 target = ScriptReadU16(script);
  bool value;

  if (bit < 0x1000) {
    value = CoreBitsetTest(bit, script->flagBits);
  } else {
    value = CoreBitsetTest(bit - 0x1000, g_scriptGlobalBits);
  }
  if (value == 0) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}
