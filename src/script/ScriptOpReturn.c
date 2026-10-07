// bdc 0x089cacf8 ScriptOpReturn
#include "bdc.h"

/* Subroutine return: pops the return address pushed by `ScriptOpCall` from the track's return
   stack (`ScriptPopReturn`) and sets `curTrack->pc` to it. */

int ScriptOpReturn(Script *script)

{
  ScriptPopReturn(script);
  return 3;
}

