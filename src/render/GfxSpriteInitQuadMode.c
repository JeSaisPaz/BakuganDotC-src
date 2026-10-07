// bdc 0x089f4014 GfxSpriteInitQuadMode
#include "bdc.h"

/* Stores `mode` in `quadMode` (`+0xe4`) without copying a template; for mode 1 it also points
   `vertices` at the shared template `0x08aa3b30` (`g_gfxQuadTemplate`). */

void GfxSpriteInitQuadMode(GfxSprite *sprite, s32 mode)
{
  if (mode == 1) {
    sprite->vertices = (GfxSpriteVertex *)&g_gfxQuadTemplate;
  }
  sprite->quadMode = mode;
}
