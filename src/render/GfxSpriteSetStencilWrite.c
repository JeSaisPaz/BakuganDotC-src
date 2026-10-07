// bdc 0x089f44b0 GfxSpriteSetStencilWrite
#include "bdc.h"

/* Makes the sprite write `ref` into the stencil buffer: when `enable`, `geStencilOp` (`+0x130`) =
   `0xdd020000` (SOP zpass = replace) and `geStencilTest` = `(ref << 8) | 0xdcff0001` (always pass);
   otherwise SOP `0xdd000000` (keep). */

void GfxSpriteSetStencilWrite(GfxSprite *sprite, bool enable, u8 ref)

{
  if (enable) {
    sprite->geStencilOp = 0xdd020000;
    sprite->geStencilTest = (uint)ref << 8 | 0xdcff0001;
    return;
  }
  sprite->geStencilOp = 0xdd000000;
  return;
}

