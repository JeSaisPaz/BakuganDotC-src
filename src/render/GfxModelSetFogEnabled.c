// bdc 0x08a29660 GfxModelSetFogEnabled
#include "bdc.h"

/* Model virtual slot 3 (`+0x1c`, base model vtable `0x08af5484` and every model-derived class):
   stores `enable` in the fog flag byte `+0xb9` that `GfxModelSetFog` sets and
   `GfxModelDlWriteState` tests. */

void GfxModelSetFogEnabled(GfxModel *self, u8 enable)

{
  self->fogEnabled = enable;
  return;
}

