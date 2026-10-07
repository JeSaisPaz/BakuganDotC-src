// bdc 0x08a15968 GmoModelGetMaterial
#include "bdc.h"

/* Returns material record `index` of a GMO model data block (0xc0-byte header from
   `GmoModelCreate`: `+0x4` nodes (0xc0 each, count `+0x18`), `+0x8` parts (0x10 each, count
   `+0x1a`), `+0xc` materials (0x10, count `+0x1c`), `+0x10` textures (0x10, count `+0x1e`), `+0x14`
   motions (0x30, count `+0x20`), flag words `+0x28`/`+0x2e`/`+0x30`) (`+0xc + index * 0x10`, bounds
   `+0x1c`); values whose upper half is non-zero (already pointers, or -1) are returned unchanged,
   NULL when out of range. */

void *GmoModelGetMaterial(GmoModel *self, u32 index)

{
  if (self != (GmoModel *)0x0) {
    if ((index + 1 & 0xffff0000) != 0) {
      return (void *)(uintptr_t)index;
    }
    if ((index & 0xffff) < (uint)self->materialCount) {
      return (void *)((u8 *)self->materials + index * 0x10);
    }
  }
  return (void *)0x0;
}

