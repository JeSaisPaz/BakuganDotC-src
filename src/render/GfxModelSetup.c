// bdc 0x089de55c GfxModelSetup
#include "bdc.h"

/* Attaches GMO data to a model object: allocates its GMO work area (`GfxModelAllocWork`,
   `workSize`), calls the virtual reset (vtable `+0x14`), binds the file (`GfxModelBindGmo`), sets
   the name (`GfxModelSetName`) when given and restores the default GMO bump region. */

typedef struct GfxVtEntry {
  short adjust;
  short pad;
  void (*fn)(void *self);
} GfxVtEntry;

typedef struct GfxSetupVt {
  unsigned char pad[0x10];
  GfxVtEntry reset;
} GfxSetupVt;

void GfxModelSetup(GfxModel *self, void *gmo, u32 gmoSize, u32 workSize, const char *name)

{
  const GfxVtEntry *e;

  GfxModelAllocWork(self,workSize);
  e = &((const GfxSetupVt *)(self->base).vtable)->reset;
  e->fn((char *)self + e->adjust);
  GfxModelBindGmo(self,gmo,gmoSize);
  if (name != (char *)0x0) {
    GfxModelSetName(self,name);
  }
  GmoSetBumpRegion((void *)0x0,(u32 *)0x0,0x2000);
  return;
}

