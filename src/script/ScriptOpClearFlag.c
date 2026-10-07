// bdc 0x089ca7f4 ScriptOpClearFlag
#include "bdc.h"

/* Clears a flag bit (`CoreBitsetClear`). */

int ScriptOpClearFlag(Script *script)

{
  u32 bit;
  
  bit = ScriptReadU16(script);
  if (bit < 0x1000) {
    CoreBitsetClear(bit,script->flagBits);
  }
  else {
    CoreBitsetClear(bit - 0x1000,g_scriptGlobalBits);
  }
  return 0;
}

