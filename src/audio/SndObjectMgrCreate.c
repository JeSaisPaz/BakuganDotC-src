// bdc 0x089c248c SndObjectMgrCreate
#include "bdc.h"

/* Lazily creates the sound-object system. When `g_soundObjectMgr` is NULL it first builds the
   slot pool (`SndObjectSlotPoolCreate``(0x1c0)`), allocates the 8-byte holder from the low heap
   (zeroed), then fills it: `holder[1]` = a `MemPool` of 0x40 items of 0x40 bytes
   (`MemPoolInit(pool, 0x40, 0x40, 1)`), the storage for `SndObject`s, and `holder[0]` = a
   0x30-byte `CoreNodeOwner` built by `CoreNodeOwnerCtor` whose vtable is then replaced with
   `g_sndObjectMgrVtbl` (the sound-object manager class). That owner is the list that
   `SndObjectCreate` appends objects to and that `SndObjectMgrUpdate` walks every frame. A
   failed pool/owner allocation leaves its slot NULL. Does nothing when the holder already exists. */

void SndObjectMgrCreate(void)

{
  bool fromLow;
  void **holder;
  MemPool *pool;
  CoreNodeOwner *owner;

  if (g_soundObjectMgr != (void **)0x0) {
    return;
  }
  SndObjectSlotPoolCreate(0x1c0);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  holder = MemAlloc(8,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_soundObjectMgr = holder;
  memset(holder,0,8);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = MemAlloc(sizeof(MemPool),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (pool != (MemPool *)0x0) {
    MemPoolInit(pool,0x40,0x40,true);
  }
  g_soundObjectMgr[1] = pool;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  owner = MemAlloc(sizeof(CoreNodeOwner),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (owner != (CoreNodeOwner *)0x0) {
    CoreNodeOwnerCtor(owner);
    owner->vtable = g_sndObjectMgrVtbl;
  }
  g_soundObjectMgr[0] = owner;
}
