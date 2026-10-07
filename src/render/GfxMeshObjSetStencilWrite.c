// bdc 0x08825670 GfxMeshObjSetStencilWrite
#include "bdc.h"

/* Makes the mesh object (`GfxMeshObjCtor`) write `ref` into the stencil buffer: when `enable`,
   `+0x1b4` = SOP 'replace on pass' (`0xdd020000`) and `+0x1b0` = STST always with ref `ref`;
   otherwise SOP keep. Counterpart of `GfxMeshObjSetStencilTest`; used by
   `GfxEffectRunCommands`. */

void GfxMeshObjSetStencilWrite(GfxMeshObj *self, bool enable, u32 ref)

{
  if (enable) {
    self->stencilOp = 0xdd020000;
    self->stencilTest = (ref & 0xff) << 8 | 0xdcff0001;
    return;
  }
  self->stencilOp = 0xdd000000;
  return;
}

