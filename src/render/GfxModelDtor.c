// bdc 0x089de31c GfxModelDtor
#include "bdc.h"

/* Destructor of the GMO model object (counterpart of `GfxModelCtor`): restores the base model
   vtable `g_gfxModelVtable`, points the GMO bump allocator at the model's own work area
   (`GmoSetBumpRegion` with `work`/`workSize`, or `workDefault` and 0x2000), releases the model
   data's instances (`GmoModelReleaseInstances(data, 0xfff5)`), clears the texture pointer of every
   texture layer and the info pointer of every material of the data, drops the data reference
   (`GmoModelRelease`), frees the owned GMO file (`gmo` when `ownsGmo`), resets the bump region,
   frees `work`, `materialStates`, `workDefault` and `chunks`, then runs the base object destructor
   (`CoreObjectDtor`) and frees the object when `flags & 1`. A null `self` does nothing. Called by 7
   model subclass destructors. */

void GfxModelDtor(GfxModel *self, u32 flags)

{
  GmoModel *data;
  void *ptr;
  s32 i;

  if (self == (GfxModel *)0) {
    return;
  }
  self->base.vtable = &g_gfxModelVtable;
  if (self->work != (void *)0) {
    GmoSetBumpRegion(self->work, (u32 *)0, self->workSize);
  }
  else {
    GmoSetBumpRegion(self->workDefault, (u32 *)0, 0x2000);
  }
  GmoModelReleaseInstances(self->data, 0xfff5);
  data = self->data;
  for (i = 0; i < (s32)data->textureCount; i++) {
    ((GmoLayer *)data->textures)[i].texture = (GmoTexture *)0;
    data = self->data;
  }
  for (i = 0; i < (s32)data->materialCount; i++) {
    ((GmoMaterial *)data->materials)[i].info = (u8 *)0;
    data = self->data;
  }
  GmoModelRelease(data);
  if (self->ownsGmo != 0 && (ptr = self->gmo) != (void *)0) {
    MemLock();
    MemFree(ptr, (const char *)0, 0);
    MemUnlock();
    self->gmo = (void *)0;
  }
  GmoSetBumpRegion((void *)0, (u32 *)0, 0x2000);
  if ((ptr = self->work) != (void *)0) {
    MemLock();
    MemFree(ptr, (const char *)0, 0);
    MemUnlock();
    self->work = (void *)0;
  }
  if ((ptr = self->materialStates) != (void *)0) {
    MemLock();
    MemFree(ptr, (const char *)0, 0);
    MemUnlock();
    self->materialStates = (void *)0;
  }
  if ((ptr = self->workDefault) != (void *)0) {
    MemLock();
    MemFree(ptr, (const char *)0, 0);
    MemUnlock();
    self->workDefault = (void *)0;
  }
  if ((ptr = self->chunks) != (void *)0) {
    MemLock();
    MemFree(ptr, (const char *)0, 0);
    MemUnlock();
    self->chunks = (void **)0;
  }
  CoreObjectDtor(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, (const char *)0, 0);
    MemUnlock();
  }
}
