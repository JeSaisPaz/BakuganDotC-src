// bdc 0x089e3594 GfxScreenCameraSetViewOffset
#include "bdc.h"

/* Sets the translation x/y of the screen camera's view matrix (`view.w.x/y`) to `xy - 0.02`
   (scrolling 2D backgrounds, `UiScreenDrawBg`). */

void GfxScreenCameraSetViewOffset(const float *xy)
{
  g_gfxScreenCamera->view.w.x = xy[0] + -0.02f;
  g_gfxScreenCamera->view.w.y = xy[1] + -0.02f;
}
