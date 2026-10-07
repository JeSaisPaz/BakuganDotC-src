// bdc 0x089c9fc4 ScriptOpWait
#include "bdc.h"

/* Waits: stores the u16 operand into `curTrack->wait` (the frame countdown of `ScriptTrack`) and
   ends the track's turn; `ScriptStep` advances `pc` and then skips the track for that many
   frames. */

int ScriptOpWait(Script *script)

{
  u32 frames;
  
  frames = ScriptReadU16(script);
  script->curTrack->wait = frames;
  return 1;
}

