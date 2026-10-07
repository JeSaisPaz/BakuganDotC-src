// bdc 0x0897e9f0 UiCollectionSphereRepositionCameras
#include "bdc.h"

/* In category 2 of `UiCollectionSphere`, re-places the seven cell cameras
   for the current page (special pages lift cell 5) after a page change. */

void UiCollectionSphereRepositionCameras(UiCollectionSphere *self)
{
  float pos[2];
  s32 i;

  if (self->category == 2) {
    for (i = 0; i < 7; i++) {
      GfxCamera *cam;
      const VtblEntry *vt;

      UiCollectionSphereSetCellYOffset(self, self->page, (u8)i);
      UiScreenToCentered(self->cellPos[i][0], self->cellPos[i][1], pos);
      GfxCameraSetScreenOffset(pos[0], pos[1] + self->cameraY, self->cameras[i]);
      GfxCameraUpdate(self->cameras[i], 0xffffffff);
      cam = (GfxCamera *)self->cameras[i];
      vt = (const VtblEntry *)cam->base.vtable;
      ((void (*)(void *))vt[2].fn)((u8 *)cam + vt[2].delta);
    }
  }
}
