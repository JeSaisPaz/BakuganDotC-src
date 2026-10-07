// bdc 0x088b3b44 StopWallCreate
#include "bdc.h"

/* Creates a "stop wall" object tagged `id`: allocates 0x90 bytes from the low heap, runs its
   constructor `StopWallCtor` (a `CoreObject`-derived class with vtable `0x08af2b84` that appends
   itself to the owner list `g_stopWallList`, created lazily by `StopWallEnsureList`), stores `id` at
   `+0x50` and builds its two quads and its collider with `StopWallBuild`. Returns the object. Called
   by `ScriptOpStopWall` (mode 0, positive float) with the id from its 2nd float operand. */

StopWall *StopWallCreate(s32 id)

{
  bool fromLow;
  StopWall *self;
  StopWall *wall;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(0x90,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  wall = (StopWall *)0x0;
  if (self != (StopWall *)0x0) {
    StopWallCtor(self);
    wall = self;
  }
  wall->id = id;
  StopWallBuild(wall,id);
  return wall;
}

