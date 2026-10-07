// bdc 0x089cac0c ScriptOpWaitUntilFlagClear
#include "bdc.h"

/* Blocks the track until the flag bit is clear (re-evaluated every frame). */

int ScriptOpWaitUntilFlagClear(Script *script)

{
  u32 bit;
  int flag;

  bit = ScriptReadU16(script);
  if (bit < 0x1000) {
    flag = CoreBitsetTest(bit, script->flagBits);
  }
  else {
    flag = CoreBitsetTest(bit - 0x1000, g_scriptGlobalBits);
  }
  if (flag == 0) {
    return 0;
  }
  return 2;
}
