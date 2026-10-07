// bdc 0x089e0b68 GfxModelSetStencilRef
#include "bdc.h"

/* Writes `value` to byte `+7` of every material state record of a GMO model object
   (`GfxModelGetMaterialState``(model, i)` for `i < materialCount`): the stencil reference value that
   `GmoDlWriteMeshRenderState` puts into the GE stencil test command (`0xdc`, `0xdcff0000 | value
   << 8 | func`). */

void GfxModelSetStencilRef(GfxModel *self, u8 value)

{
  GfxMaterialState *state;
  int index;
  int count;

  count = self->materialCount;
  index = 0;
  if (0 < count) {
    do {
      state = (GfxMaterialState *)GfxModelGetMaterialState(self, index);
      index = index + 1;
      state->stencilRef = value;
    } while (index < count);
  }
  return;
}
