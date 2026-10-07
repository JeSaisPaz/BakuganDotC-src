// bdc 0x089c26f0 SndObjectCreate
#include "bdc.h"

/* Creates a sound object with `slotCount` slots and appends it to the sound-object list `owner`:
   takes a 0x40-byte item from the pool `g_soundObjectMgr[1]` (`MemPoolAlloc`) or, when the pool
   is missing or full, a 0x40-byte block from the high end of the heap, runs the sound-object
   constructor `SndObjectInit(obj, slotCount)` and links it with `CoreNodeOwnerAppend`. Returns
   the object, or NULL when the allocation failed. */

CoreNode *SndObjectCreate(CoreNodeOwner *owner, s32 slotCount)

{
  bool fromLow;
  SndObject *self;
  SndObject *self_00;
  
  self_00 = (SndObject *)0x0;
  if (g_soundObjectMgr[1] != (MemPool *)0x0) {
    self_00 = MemPoolAlloc(g_soundObjectMgr[1]);
  }
  if (self_00 == (SndObject *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(false);
    self = MemAlloc(sizeof(SndObject),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self_00 = (SndObject *)0x0;
    if (self != (SndObject *)0x0) {
      SndObjectInit(self,slotCount);
      self_00 = self;
    }
  }
  else {
    SndObjectInit(self_00,slotCount);
  }
  CoreNodeOwnerAppend(owner,&self_00->node);
  return &self_00->node;
}

