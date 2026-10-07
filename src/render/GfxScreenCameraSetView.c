// bdc 0x089e31fc GfxScreenCameraSetView
#include "bdc.h"

/* Copies a 4x4 matrix into the screen camera's view matrix (`g_gfxScreenCamera->view`); used by
   `GfxSpriteLayerDraw2D`. */

void GfxScreenCameraSetView(const ScePspFMatrix4 *m)
{
  g_gfxScreenCamera->view = *m;
}
