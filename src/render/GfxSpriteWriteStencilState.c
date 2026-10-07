// bdc 0x089f44ec GfxSpriteWriteStencilState
#include "bdc.h"

/* Writes the sprite's GE stencil words into a display list: `dl[0] = geStencilOp` (`+0x130`),
   `dl[1] = geStencilTest` (`+0x12c`); returns `dl + 2`. */

u32 *GfxSpriteWriteStencilState(GfxSprite *sprite, u32 *dl)

{
  *dl = sprite->geStencilOp;
  dl[1] = sprite->geStencilTest;
  return dl + 2;
}

