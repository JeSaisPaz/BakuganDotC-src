// bdc 0x08a2c024 GameStageLayoutObjDtor
#include "bdc.h"

/* Destructor (vtable `0x08af6db0` entry 1) of the 0x40-byte stage layout object that
   `GameStageSpawnLayoutObjects` creates for each layout record (factory
   `ActorStageObjRecordAdd`, ctor `ActorStageObjRecordCtor`, list `0x08abd620`): reinstalls the
   vtable, runs `CoreObjectDtor` and frees the object when `flags & 1`. */

void GameStageLayoutObjDtor(CoreObject *obj, u32 flags)

{
  if (obj != (CoreObject *)0x0) {
    obj->vtable = g_gameStageLayoutObjVtbl;
    CoreObjectDtor(obj,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

