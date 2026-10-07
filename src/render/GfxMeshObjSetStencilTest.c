// bdc 0x08825630 GfxMeshObjSetStencilTest
#include "bdc.h"

/* Sets the stencil test of the mesh object (`GfxMeshObjCtor`): when `enable`, `+0x1b0` = STST
   'equal `ref`' (`0xdcff0002 | ref << 8`) and `+0x1b4` = SOP keep; otherwise STST 'always'
   (`0xdcff0001`). Used by `GfxEffectRunCommands` (masking effects to a stencil shape). */

void GfxMeshObjSetStencilTest(GfxMeshObj *self, bool enable, u32 ref)

{
  if (enable) {
    self->stencilTest = (ref & 0xff) << 8 | 0xdcff0002;
    self->stencilOp = 0xdd000000;
    return;
  }
  self->stencilTest = 0xdcff0001;
  return;
}

