// bdc 0x08825404 GfxMeshObjDtor
#include "bdc.h"

/* Destructor of the mesh object (`GfxMeshObjCtor`) (`g_gfxMeshObjVtbl` slot 1): resets the
   vtable, frees `buffer` (`+0xf8`), `indices` (`+0xf0`) when `self->flags & 2` says it owns it,
   `vertexBuffer` (`+0x19c`) when `self->flags & 1`, destroys the effect chain (`GfxEffectChainDtor`,
   flags 2) and the `CoreObject` base, and frees the object when `flags & 1`. NULL is ignored. */

void GfxMeshObjDtor(GfxMeshObj *self, u32 flags)
{
  void *ptr;

  if (self == (GfxMeshObj *)0x0) {
    return;
  }
  ptr = self->buffer;
  self->base.vtable = g_gfxMeshObjVtbl;
  if (ptr != (void *)0x0) {
    MemLock();
    MemFree(ptr, (const char *)0x0, 0);
    MemUnlock();
    self->buffer = (void *)0x0;
  }
  if ((self->flags & 2) != 0) {
    ptr = self->indices;
    if (ptr != (void *)0x0) {
      MemLock();
      MemFree(ptr, (const char *)0x0, 0);
      MemUnlock();
      self->indices = (void *)0x0;
    }
  }
  if ((self->flags & 1) != 0) {
    ptr = self->vertexBuffer;
    if (ptr != (void *)0x0) {
      MemLock();
      MemFree(ptr, (const char *)0x0, 0);
      MemUnlock();
      self->vertexBuffer = (void *)0x0;
    }
  }
  GfxEffectChainDtor(&self->effectChain, 2);
  CoreObjectDtor(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, (const char *)0x0, 0);
    MemUnlock();
  }
}
