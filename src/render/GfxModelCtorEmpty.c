// bdc 0x089de268 GfxModelCtorEmpty
#include "bdc.h"

/* Constructor of a GMO model object without a model file: `CoreObjectInit` (no chain), model
   vtable `0x08af5484`, clears the list fields and initialises all model fields
   (`GfxModelInitFields`). The file is attached later (`GfxModelSetup`). */

GfxModel *GfxModelCtorEmpty(GfxModel *self)

{
  CoreObjectInit(&self->base,(CoreObject *)0x0);
  (self->base).vtable = &g_gfxModelVtable;
  self->workDefault = (void *)0x0;
  self->chunks = (void **)0x0;
  self->work = (void *)0x0;
  GfxModelInitFields(self);
  return self;
}

