// bdc 0x089cb074 ScriptOpIgnoreU16
#include "bdc.h"

/* Reads one u16 operand and discards it (a no-op that still consumes an operand). */

int ScriptOpIgnoreU16(Script *script)

{
  ScriptReadU16(script);
  return 0;
}

