// bdc 0x089cad18 ScriptOpRunScript
#include "bdc.h"

/* Runs another script and waits for it to finish: the first execution spawns the named script
   (`ScriptSpawn`) as a child and links it to the current track; the track then re-executes this
   instruction every frame (return 2) until the child has ended. */

int ScriptOpRunScript(Script *script)
{
  u8 state;
  int ret;
  const char *name;
  Script *child;

  state = script->trackLocals[script->track].childState;
  ret = 0;
  if (state == 0) {
    name = (const char *)script->operand;
    ScriptSkipString(script);
    ScriptMngGet();
    child = ScriptSpawn(name);
    child->parentState = &script->trackLocals[script->track].childState;
    script->trackLocals[script->track].childState = 1;
    ret = 2;
  } else if (state < 2) {
    ret = 2;
  } else if (state < 3) {
    script->trackLocals[script->track].childState = 0;
  }
  return ret;
}
