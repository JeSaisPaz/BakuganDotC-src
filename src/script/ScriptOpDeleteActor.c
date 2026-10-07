// bdc 0x0880ea60 ScriptOpDeleteActor
#include "bdc.h"

/* Script opcode: reads an actor ref and, when it is still in the actor chain
   (`ActorListContains`), schedules its deletion with `CoreObjectDeferDelete``(actor, 1)`.
   Returns 0. */

int ScriptOpDeleteActor(Script *script)

{
  u32 *ref;
  CoreObject *obj;
  
  ref = ScriptReadRef(script,2);
  obj = ActorListContains((void *)(uintptr_t)*ref);
  if (obj != (CoreObject *)0x0) {
    CoreObjectDeferDelete(obj,1);
  }
  return 0;
}

