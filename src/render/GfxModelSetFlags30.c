// bdc 0x089dfdac GfxModelSetFlags30
#include "bdc.h"

/* Sets (`on`) or clears the bits `mask` of the flag word `+0x30` of the model's GMO data (`+0x130`)
   with `GmoModelSetFlags30`. */

void GfxModelSetFlags30(GfxModel *self, u32 mask, bool on)

{
  if (on) {
    GmoModelSetFlags30(self->data,mask,mask);
    return;
  }
  GmoModelSetFlags30(self->data,mask,0);
  return;
}

