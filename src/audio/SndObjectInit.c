// bdc 0x089c1cd8 SndObjectInit
#include "bdc.h"

/* Constructor of the `SndObject`. Runs the `CoreNode` base constructor (`CoreNodeCtor`, no
   anchor), installs the SndObject vtable `g_sndObjectVtable`, then allocates the array of
   `emitterCount` emitter pointers: first from `g_soundObjectSlotPool` (`MemPoolAllocN` takes
   `emitterCount` consecutive 4-byte items) and, if the pool is missing or full, from the top of the
   heap (`MemSetAllocFromLow(0)`, `emitterCount * 4` bytes). The array is zeroed; `emitterCount` is
   stored, `alive = 1` and `src = NULL`. Returns `self`. */

SndObject *SndObjectInit(SndObject *self, s32 emitterCount)
{
  bool fromLow;
  SndEmitter **emitters;

  CoreNodeCtor(&self->node, NULL);
  self->node.vtable = g_sndObjectVtable;
  if (g_soundObjectSlotPool == NULL) {
    self->emitters = NULL;
    emitters = NULL;
  } else {
    emitters = MemPoolAllocN(g_soundObjectSlotPool, emitterCount);
    self->emitters = emitters;
  }
  if (emitters == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(false);
    emitters = MemAlloc(emitterCount * 4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->emitters = emitters;
  } else {
    self->emitters = emitters;
  }
  memset(emitters, 0, emitterCount * 4);
  self->emitterCount = emitterCount;
  self->alive = 1;
  self->src = NULL;
  return self;
}
