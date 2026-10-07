// bdc 0x08a15044 GmoModelRelease
#include "bdc.h"

/* Drops one reference of a GMO model data block (0xc0-byte header from `GmoModelCreate`: `+0x4`
   nodes (0xc0 each, count `+0x18`), `+0x8` parts (0x10 each, count `+0x1a`), `+0xc` materials
   (0x10, count `+0x1c`), `+0x10` textures (0x10, count `+0x1e`), `+0x14` motions (0x30, count
   `+0x20`), flag words `+0x28`/`+0x2e`/`+0x30`); at 0 destroys its contents
   (`GmoModelDestroyContents`) and frees it. Called by `GfxModelDtor`. Returns `self`. */

GmoModel *GmoModelRelease(GmoModel *self)

{
  if (self != (GmoModel *)0x0) {
    self->refCount = self->refCount - 1;
    if (self->refCount == 0) {
      GmoModelDestroyContents(self);
      GmoHeapReleaseThunk(0, self);
    }
  }
  return self;
}
