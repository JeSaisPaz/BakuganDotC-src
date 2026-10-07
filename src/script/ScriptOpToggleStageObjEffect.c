// bdc 0x088108f4 ScriptOpToggleStageObjEffect
#include "bdc.h"

/* Script opcode: reads an s16 instance id, a u16 kind, a u32 mode and four floats, finds the stage
   object with `ActorStageObjFind` and re-checks it with `ActorStageObjValidate`. If the object
   is live, mode is 0 and its virtual predicate (vtable `+0x14`, slot 17 at `+0x88`) returns
   non-zero, the first float switches it: `flag <= 0` calls `ActorStageObjDeactivate`, otherwise
   (incl. NaN) `ActorStageObjActivate`. The other three floats are read and discarded. Always
   returns 0. */

int ScriptOpToggleStageObjEffect(Script *script)
{
  u32 instanceId;
  u32 kind;
  u32 mode;
  ActorStageObjBase *obj;
  float flag;
  const VtblEntry *pred;

  instanceId = ScriptReadU16(script);
  kind = ScriptReadU16(script);
  obj = ActorStageObjFind(kind, (int)(short)instanceId);
  mode = ScriptReadU32(script);
  flag = ScriptReadFloat(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  obj = ActorStageObjValidate(obj);
  if (obj != 0 && mode == 0) {
    pred = &((const VtblEntry *)obj->base.base.vtable)[17];
    if (((int (*)(void *))pred->fn)((u8 *)obj + pred->delta) != 0) {
      if (flag <= 0.0f) {
        ActorStageObjDeactivate((ActorStageObjLandmark *)obj);
      } else {
        ActorStageObjActivate(obj);
      }
    }
  }
  return 0;
}
