// bdc 0x089c1de4 SndObjectDestroy
#include "bdc.h"

/* Virtual destructor of `SndObject` (slot 1 of the vtable `0x08af525c`, called by
   `SndObjectMgrUpdate` with flags 2 for pool objects and 3 for heap objects). A NULL `self` is
   ignored. It resets the vtable to its own, frees the emitter-pointer array (back to
   `g_soundObjectSlotPool` with `MemPoolFreeN``(pool, array, count)`; when the array did not
   come from the pool, with `MemFree`), runs the `CoreNode` destructor (`CoreNodeDtor`, which
   unlinks the node from its `CoreNodeOwner`) and, if `flags & 1`, frees the object itself with
   `MemFree`. */

void SndObjectDestroy(SndObject *self, u32 flags)

{
  if (self == (SndObject *)0x0) {
    return;
  }
  (self->node).vtable = g_sndObjectVtable;
  if (g_soundObjectSlotPool != (MemPool *)0x0) {
    if (MemPoolFreeN(g_soundObjectSlotPool,self->emitters,self->emitterCount)) {
      self->emitters = (SndEmitter **)0x0;
    }
  }
  if (self->emitters != (SndEmitter **)0x0) {
    SndEmitter **ptr = self->emitters;

    MemLock();
    MemFree(ptr,(char *)0x0,0);
    MemUnlock();
    self->emitters = (SndEmitter **)0x0;
  }
  CoreNodeDtor(&self->node,0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self,(char *)0x0,0);
    MemUnlock();
  }
  return;
}
