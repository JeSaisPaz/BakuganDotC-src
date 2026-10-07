// bdc 0x088d8e30 GameFieldPointCreate
#include "bdc.h"

/* Allocates (low heap) and constructs a field point (`GameFieldPointCtor`), fills it with `pos`,
   `kind`, `id` and `heading` (`GameFieldPointSet`; the float stays in `f12`), creates the point
   list `g_gameFieldPointList` on first use (`GameFieldPointInitList`) and appends the point to it
   (`CoreObjectListAppend`). Returns the point. */

CoreObject *GameFieldPointCreate(const float *pos, s32 kind, s32 id, float heading)

{
  bool fromLow;
  CoreObject *obj;
  CoreObject *obj_00;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  obj = MemAlloc(sizeof(GameFieldPoint),NULL,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  obj_00 = NULL;
  if (obj != NULL) {
    GameFieldPointCtor(obj);
    obj_00 = obj;
  }
  GameFieldPointSet(obj_00,pos,kind,id,heading);
  if (g_gameFieldPointList == NULL) {
    GameFieldPointInitList();
  }
  CoreObjectListAppend(obj_00, (CoreObjectList *)g_gameFieldPointList);
  return obj_00;
}

