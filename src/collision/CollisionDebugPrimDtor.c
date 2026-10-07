// bdc 0x089efbbc CollisionDebugPrimDtor
#include "bdc.h"

/* Destructor of a debug primitive: clears the chain head `0x08ac5da8` if it points to it, runs
   `CoreObjectDtor` and frees it when `flags & 1`. */

void CollisionDebugPrimDtor(CollisionDebugPrim *self, u32 flags)

{
  if (self != (CollisionDebugPrim *)0x0) {
    (self->base).vtable = g_collisionDebugPrimVtbl;
    if (g_collisionDebugPrims == self) {
      g_collisionDebugPrims = (CollisionDebugPrim *)0x0;
    }
    CoreObjectDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

