// bdc 0x089cacc0 ScriptOpCall
#include "bdc.h"

/* Subroutine call: pushes the address of the next instruction (`curTrack->pc + length`) on the
   track's return stack in `ScriptTrackLocal` (`ScriptPushReturn`) and jumps to the target. */

int ScriptOpCall(Script *script)

{
  ScriptPushReturn(script);
  script->curTrack->pc = (u16)ScriptReadU16(script);
  return 3;
}

