// bdc 0x089ca048 ScriptOpSetVar
#include "bdc.h"

/* Assigns an immediate (or another variable) to a script variable: `*var = value`. */

int ScriptOpSetVar(Script *script)

{
  u32 *var;
  
  var = ScriptReadRef(script,2);
  *var = ScriptReadU32(script);
  return 0;
}

