// bdc 0x089e6808 CollisionPhysBoxDtor
#include "bdc.h"

/* Destructor of the physics box: frees its 0x180-byte particle buffer and the box itself when
   `flags & 1` (vtable `g_collisionPhysBoxVtbl` slot `+0xc`). */

void CollisionPhysBoxDtor(CollisionPhysBox *self, u32 flags)

{
  ScePspFVector4 *ptr;
  
  if (self != (CollisionPhysBox *)0x0) {
    ptr = self->pos;
    self->vtbl = g_collisionPhysBoxVtbl;
    if (ptr != (ScePspFVector4 *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      self->pos = (ScePspFVector4 *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

