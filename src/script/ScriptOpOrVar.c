// bdc 0x089ca3b0 ScriptOpOrVar
#include "bdc.h"

/* Bitwise operation on a script variable: `*var |= value`. */

int ScriptOpOrVar(Script *script)

{
  u32 *var;
  u32 value;
  
  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  *var = *var | value;
  return 0;
}

