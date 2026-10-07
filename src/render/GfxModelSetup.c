// bdc 0x089de55c GfxModelSetup
#include "bdc.h"

/* Attaches GMO data to a model object: allocates its GMO work area (`GfxModelAllocWork`,
   `workSize`), calls the virtual reset (vtable `+0x14`), binds the file (`GfxModelBindGmo`), sets
   the name (`GfxModelSetName`) when given and restores the default GMO bump region. */

void GfxModelSetup(GfxModel *self, void *gmo, u32 gmoSize, u32 workSize, const char *name)

{
  const VtblEntry *e;

  GfxModelAllocWork(self,workSize);
  e = &((const VtblEntry *)(self->base).vtable)[2];
  ((void (*)(void *))e->fn)((char *)self + e->delta);
  GfxModelBindGmo(self,gmo,gmoSize);
  if (name != (char *)0x0) {
    GfxModelSetName(self,name);
  }
  GmoSetBumpRegion((void *)0x0,(u32 *)0x0,0x2000);
  return;
}

