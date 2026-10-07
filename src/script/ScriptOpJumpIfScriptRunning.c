// bdc 0x089cb1a4 ScriptOpJumpIfScriptRunning
#include "bdc.h"

/* Conditional jump: jumps to the target if a script with the inline name is in `g_scriptList`
   (case-insensitive name lookup, `ScriptFindByName`). */

int ScriptOpJumpIfScriptRunning(Script *script)

{
  const char *name;
  u32 target;
  
  name = (const char *)script->operand;
  ScriptSkipString(script);
  target = ScriptReadU16(script);
  if (ScriptFindByName(name) != (Script *)0x0) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}

