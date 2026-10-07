// bdc 0x089f4470 GfxSpriteSetStencilTest
#include "bdc.h"

/* Enables or disables the sprite's stencil test: when `enable`, `geStencilTest` (`+0x12c`) = `(ref
   << 8) | 0xdcff0002` (STST func 2 = EQUAL? with mask 0xff) and `geStencilOp` (`+0x130`) =
   `0xdd000000` (keep); otherwise STST `0xdcff0001` (always pass). */

void GfxSpriteSetStencilTest(GfxSprite *sprite, bool enable, u8 ref)

{
  if (enable) {
    sprite->geStencilTest = (uint)ref << 8 | 0xdcff0002;
    sprite->geStencilOp = 0xdd000000;
    return;
  }
  sprite->geStencilTest = 0xdcff0001;
  return;
}

