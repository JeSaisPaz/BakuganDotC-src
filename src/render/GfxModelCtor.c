// bdc 0x089de2b4 GfxModelCtor
#include "bdc.h"

/* Base constructor of the in-world GMO model object: runs `CoreObjectInit` with no chain,
   installs the model vtable `0x08af5484`, clears two list fields (`[0xb].unk10` and `[10].unk08` of
   the base layout) and calls `GfxModelLoadByName(model, gmoName, flags)` which loads the model file. The
   object's model data pointer then lives at `+0x130`, its node table (count `+0xe8`, names `+0xfc`)
   and material table (count `+0xf0`, names `+0x104`) are used by the `GfxModelFindNode` family.
   Returns `model`. */

GfxModel *GfxModelCtor(GfxModel *self, const char *gmoName, u32 flags)

{
  CoreObjectInit(&self->base,(CoreObject *)0x0);
  (self->base).vtable = &g_gfxModelVtable;
  self->workDefault = (void *)0x0;
  self->chunks = (void **)0x0;
  GfxModelLoadByName(self,gmoName,flags);
  return self;
}

