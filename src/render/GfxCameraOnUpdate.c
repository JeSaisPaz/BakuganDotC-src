// bdc 0x08a2d1b8 GfxCameraOnUpdate
#include "bdc.h"

/* Empty virtual slot 2 (`+0x14`) of the camera class (vtable `0x08af54d4`): the hook the UI scenes
   call right after `GfxCameraUpdate` when they set up a camera (e.g.
   `UiAdvSelectCreateCamera`); the base camera does nothing. */

void GfxCameraOnUpdate(GfxCamera *camera)

{
  return;
}

