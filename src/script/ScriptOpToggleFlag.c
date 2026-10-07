// bdc 0x089ca848 ScriptOpToggleFlag
#include "bdc.h"

/* Toggles a flag bit (`CoreBitsetToggle`). */

int ScriptOpToggleFlag(Script *script)

{
  u32 bit;
  
  bit = ScriptReadU16(script);
  if (bit < 0x1000) {
    CoreBitsetToggle(bit,script->flagBits);
  }
  else {
    CoreBitsetToggle(bit - 0x1000,g_scriptGlobalBits);
  }
  return 0;
}

