// bdc 0x089e2fd8 GfxCameraSetScreenOffset
#include "bdc.h"

/* Sets the camera's screen-space offset (`+0x15c`, `+0x160`), used e.g. by the shake
   (`GfxCameraUpdateShake`) and the UI model viewers. */

void GfxCameraSetScreenOffset(float x, float y, GfxCamera *cam)

{
  cam->screenOffset[0] = x;
  cam->screenOffset[1] = y;
  return;
}

