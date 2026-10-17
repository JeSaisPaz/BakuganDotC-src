// bdc 0x0880e984 ScriptOpJumpIfStageWordEq
#include "bdc.h"

/* Conditional jump on the per-stage profile word: reads u32 `value` and u16 `target`; if `*(u32
   *)(*profile + 0x250 + stage*4)` (stage = script global 1, `*0x08ac58c4 + 4`) equals `value`, sets
   the track pc (`track+6`) to `target` and returns 3; otherwise 0. */

int ScriptOpJumpIfStageWordEq(Script *script)

{
  u32 value;
  u32 target;
  SaveProfile *profile;
  
  value = ScriptReadU32(script);
  target = ScriptReadU16(script);
  profile = SaveGetProfile();
  /* PC port (class n): indexed through a word pointer, as the MIPS does: stage numbers run past the 32 words (stage 0x25 lands in stageActorTags, +0x2e4, on the PSP too); the array index was out of bounds for UBSan. */
  if (((s32 *)profile->data->stageStates)[g_scriptGlobalVars[1]] == value) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}

