// bdc 0x089cadf4 ScriptOpRunScriptFmt
#include "bdc.h"

/* Formatted variant of `ScriptOpRunScript`: the child name is built with `sprintf` from an inline
   format string and one (op 0x24) or two (op 0x25, the handler tests `script->opcode == '%'`)
   values, then spawned and awaited as in `ScriptOpRunScript`. Returns 2 while the child runs
   (spawn and wait states), 0 once it has finished (state 2, reset to 0) or for any other state. */

int ScriptOpRunScriptFmt(Script *script)

{
  u8 state;
  const char *format;
  u32 a;
  u32 b;
  Script *child;
  char name[256];

  state = script->trackLocals[script->track].childState;
  if (state == 0) {
    if (script->opcode == 0x25) {
      format = (const char *)script->operand;
      ScriptSkipString(script);
      a = ScriptReadU32(script);
      b = ScriptReadU32(script);
      sprintf(name, format, a, b);
    }
    else {
      format = (const char *)script->operand;
      ScriptSkipString(script);
      a = ScriptReadU32(script);
      sprintf(name, format, a);
    }
    ScriptMngGet();
    child = ScriptSpawn(name);
    child->parentState = &script->trackLocals[script->track].childState;
    script->trackLocals[script->track].childState = 1;
    return 2;
  }
  if (state == 1) {
    return 2;
  }
  if (state == 2) {
    script->trackLocals[script->track].childState = 0;
  }
  return 0;
}
