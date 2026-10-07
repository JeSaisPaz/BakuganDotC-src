// bdc 0x089c9f5c ScriptPopReturn
#include "bdc.h"

/* Pops the return address pushed by `ScriptPushReturn`: decrements `callDepth` and sets
   `curTrack->pc` to `callStack[callDepth]`. No underflow check. */

void ScriptPopReturn(Script *script)

{
  script->trackLocals[script->track].callDepth = script->trackLocals[script->track].callDepth + -1;
  script->curTrack->pc =
       script->trackLocals[script->track].callStack[script->trackLocals[script->track].callDepth];
  return;
}

