// bdc 0x089ca36c ScriptOpAndVar
#include "bdc.h"

/* Bitwise operation on a script variable: `*var &= value`. */

int ScriptOpAndVar(Script *script)

{
  u32 *var;
  u32 value;
  
  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  *var = *var & value;
  return 0;
}

