// bdc 0x088bf0e8 GameFieldDrawSpriteLayerRelative
#include "bdc.h"

/* Draws the sprite/billboard layer `layer` (`GfxSpriteLayerDrawWorld` with the field camera) between
   `GameFieldCameraRecenterBegin` and `GameFieldCameraRecenterEnd`; nothing when `layer` is
   NULL. */

void GameFieldDrawSpriteLayerRelative(CoreTask *task, void *dl, void *layer)
{
  float savedEye[4] __attribute__((aligned(16)));

  if (layer != NULL) {
    GameFieldCameraRecenterBegin(savedEye);
    GfxSpriteLayerDrawWorld(layer, dl, g_gfxActiveCamera, savedEye);
    GameFieldCameraRecenterEnd(savedEye);
  }
}
