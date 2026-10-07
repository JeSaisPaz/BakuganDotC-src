// bdc 0x088d3af8 GameStagePropMaterialCallback
#include "bdc.h"

/* Per-material callback used by `GameStagePropFixMaterials`: in the material's flag byte `+4`,
   sets mode 8 in bits 2..3 when they are 0 and also sets bits 2..3 when bits 6..7 equal 0x80. */

void GameStagePropMaterialCallback(void *material)
{
  u8 *flags = &((GfxMaterialState *)material)->renderFlags;
  u8 b = *flags;

  if ((b & 0xc) == 0) {
    *flags = (b & 0xf3) | 8;
    b = *flags;
  }
  if ((b & 0xc0) == 0x80) {
    *flags = b | 0xc;
  }
}
