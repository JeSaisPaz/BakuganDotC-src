// bdc 0x089ca7a0 ScriptOpSetFlag
#include "bdc.h"

/* Sets a flag bit (`CoreBitsetSet`). */

int ScriptOpSetFlag(Script *script)

{
  u32 bit;
  
  bit = ScriptReadU16(script);
  if (bit < 0x1000) {
    CoreBitsetSet(bit,script->flagBits);
  }
  else {
    CoreBitsetSet(bit - 0x1000,g_scriptGlobalBits);
  }
  return 0;
}

