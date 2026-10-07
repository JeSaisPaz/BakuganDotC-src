// bdc 0x089c9efc ScriptPushReturn
#include "bdc.h"

/* Pushes a return address for `ScriptOpCall`: stores `curTrack->pc + length` (the next
   instruction) into the track's return stack in `ScriptTrackLocal` (`callStack[callDepth]`) and
   increments `callDepth`. No bounds check. */

void ScriptPushReturn(Script *script)
{
  ScriptTrackLocal *local = &script->trackLocals[script->track];

  local->callStack[local->callDepth] = script->curTrack->pc + script->length;
  script->trackLocals[script->track].callDepth = script->trackLocals[script->track].callDepth + 1;
}
