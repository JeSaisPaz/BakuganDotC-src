// bdc 0x089ca10c ScriptOpMulVar
#include "bdc.h"

/* Arithmetic on a script variable: `*var *= value` (signed 32-bit). */

int ScriptOpMulVar(Script *script)

{
  u32 *var;
  u32 value;

  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  *var = *var * value;
  return 0;
}

