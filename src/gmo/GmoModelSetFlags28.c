// bdc 0x08a158f4 GmoModelSetFlags28
#include "bdc.h"

/* Sets the bits `mask` of the word `+0x28` of a GMO model data block (0xc0-byte header from
   `GmoModelCreate`: `+0x4` nodes (0xc0 each, count `+0x18`), `+0x8` parts (0x10 each, count
   `+0x1a`), `+0xc` materials (0x10, count `+0x1c`), `+0x10` textures (0x10, count `+0x1e`), `+0x14`
   motions (0x30, count `+0x20`), flag words `+0x28`/`+0x2e`/`+0x30`) to `value`. */

void GmoModelSetFlags28(GmoModel *self, u32 mask, u32 value)

{
  if (self != (GmoModel *)0x0) {
    self->flags28 = self->flags28 & ~mask | mask & value;
  }
  return;
}

