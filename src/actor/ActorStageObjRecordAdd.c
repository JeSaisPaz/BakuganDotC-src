// bdc 0x088b2980 ActorStageObjRecordAdd
#include "bdc.h"

/* Creates a stage layout spawn record (0x40 bytes, low heap, `ActorStageObjRecordCtor`), fills it
   (`ActorStageObjRecordSet`) and appends it to the record list `0x08abd620` (initialised by
   `ActorStageObjRecordInitLists` if needed). Called by the layout loaders
   `BtlStageSpawnPlacedObjects` and `GameStageSpawnLayoutObjects`. */

void *ActorStageObjRecordAdd(float *pos, s16 kind, s16 arg, s16 type, s16 variant)

{
  bool fromLow;
  CoreObject *rec;
  CoreObject *obj;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  rec = MemAlloc(0x40,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  obj = (CoreObject *)0x0;
  if (rec != (CoreObject *)0x0) {
    ActorStageObjRecordCtor(rec);
    obj = rec;
  }
  ActorStageObjRecordSet(obj,pos,kind,arg,type,variant);
  if (g_stageObjRecordList == (void **)0x0) {
    ActorStageObjRecordInitLists();
  }
  CoreObjectListAppend(obj,(CoreObjectList *)g_stageObjRecordList);
  return obj;
}

