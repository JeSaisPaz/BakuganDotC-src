// bdc 0x089ca154 ScriptOpDivVar
#include "bdc.h"

/* Arithmetic on a script variable: `*var /= value` (signed 32-bit). */

int ScriptOpDivVar(Script *script)

{
  u32 *var;
  u32 value;

  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  *var = (int)*var / (int)value;
  return 0;
}

