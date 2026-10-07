// bdc 0x089f5988 GfxSpriteLayerResetView
#include "bdc.h"

/* Resets the layer's view matrix (`+0x30..+0x6f`) to identity with a translation of (-0.02, -0.02)
   (`0xbca3d70a`), a small sub-pixel bias. */

void GfxSpriteLayerResetView(GfxSpriteLayer *self)
{
  ScePspFMatrix4 *view = &self->view;

  view->x.x = 1.0f;
  view->x.y = 0.0f;
  view->x.z = 0.0f;
  view->x.w = 0.0f;
  view->y.x = 0.0f;
  view->y.y = 1.0f;
  view->y.z = 0.0f;
  view->y.w = 0.0f;
  view->z.x = 0.0f;
  view->z.y = 0.0f;
  view->z.z = 1.0f;
  view->z.w = 0.0f;
  view->w.x = 0.0f;
  view->w.y = 0.0f;
  view->w.z = 0.0f;
  view->w.w = 1.0f;
  view->w.x = -0.02f;
  view->w.y = -0.02f;
}
