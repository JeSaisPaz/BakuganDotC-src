// bdc 0x088b3540 StopWallDtor
#include "bdc.h"

/* Destructor (vtable `0x08af2b84` slot 1) of a stop wall (`StopWallCtor`): deletes the collider
   `+0x18` through its virtual destructor (flags 3), frees the shape block `+0x1c`, then runs
   `CoreObjectDtor`. (GCC 2.x deleting destructor: frees the object when bit 0 of `flags` is set.)
    */

void StopWallDtor(StopWall *self, u32 flags)
{
  void *obj;
  VtblEntry *e;

  if (self != NULL) {
    obj = self->collider;
    self->base.vtable = g_stopWallVtbl;
    if (obj != NULL) {
      e = (VtblEntry *)((CoreNode *)obj)->vtable + 1;
      ((void (*)(void *, int))e->fn)((char *)obj + e->delta, 3);
      self->collider = NULL;
    }
    obj = self->shape;
    if (obj != NULL) {
      MemLock();
      MemFree(obj, NULL, 0);
      MemUnlock();
      self->shape = NULL;
    }
    CoreObjectDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, NULL, 0);
      MemUnlock();
    }
  }
}
