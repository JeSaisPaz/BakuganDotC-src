// bdc 0x089ca3f4 ScriptOpXorVar
#include "bdc.h"

/* Bitwise operation on a script variable: `*var ^= value`. */

int ScriptOpXorVar(Script *script)

{
  u32 *var;
  u32 value;
  
  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  *var = *var ^ value;
  return 0;
}

