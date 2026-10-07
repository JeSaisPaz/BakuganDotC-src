// bdc 0x089ca19c ScriptOpJump
#include "bdc.h"

/* Unconditional jump: sets `curTrack->pc` to the u16 operand. */

int ScriptOpJump(Script *script)

{
  u32 target;
  
  target = ScriptReadU16(script);
  script->curTrack->pc = (u16)target;
  return 3;
}

