// bdc 0x089caba8 ScriptOpWaitUntilFlagSet
#include "bdc.h"

/* Blocks the track until the flag bit is set (re-evaluated every frame). */

int ScriptOpWaitUntilFlagSet(Script *script)

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
  if (flag == 1) {
    return 0;
  }
  return 2;
}
