// bdc 0x08811218 ScriptOpBranchIfStageObjFlag281
#include "bdc.h"

/* Script opcode: conditional jump on a stage object (`ActorStageObjFind` by kind and instance
   id): compares its `dead` byte (`+0x281`, 0 = alive; non-zero entries are skipped by the list
   searches) against a float with a comparison code and jumps to a u16 target when true (returns
   3, else 0). A missing object always takes the jump; a non-zero mode forces code 10 (no
   comparison). */

int ScriptOpBranchIfStageObjFlag281(Script *script)
{
  u32 instanceId;
  u32 kind;
  ActorStageObjBase *obj;
  u32 mode;
  u32 cmp;
  float ref;
  u32 target;
  float value;
  int take;

  instanceId = ScriptReadU16(script);
  kind = ScriptReadU16(script);
  obj = (ActorStageObjBase *)ActorStageObjFind(kind, (int)(short)instanceId);
  mode = ScriptReadU32(script);
  cmp = ScriptReadU16(script);
  ref = ScriptReadFloat(script);
  target = ScriptReadU16(script);
  value = 0.0f;
  take = 0;
  if (obj == NULL) {
    take = 1;
  } else if (mode == 0) {
    value = (float)obj->dead;
  } else {
    cmp = 10;
  }
  switch (cmp) {
  case 0:
    if (value < ref) {
      take = 1;
    }
    break;
  case 1:
    if (value <= ref) {
      take = 1;
    }
    break;
  case 2:
    if (value == ref) {
      take = 1;
    }
    break;
  case 3:
    if (!(value == ref)) {
      take = 1;
    }
    break;
  case 4:
    if (!(value <= ref)) {
      take = 1;
    }
    break;
  case 5:
    if (!(value < ref)) {
      take = 1;
    }
    break;
  }
  if (!take) {
    return 0;
  }
  script->curTrack->pc = (u16)target;
  return 3;
}
