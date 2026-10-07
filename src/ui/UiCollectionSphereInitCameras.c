// bdc 0x0897a078 UiCollectionSphereInitCameras
#include "bdc.h"

/* Initialises the seven cell cameras of `UiCollectionSphere`
   (`cameras[i]`, `GfxCameraInit`): resets each one, sets its near Z to 1 and its eye and target
   to the origin, converts the cell anchor (`cellPos[i]`) to centre-relative coordinates
   (`UiScreenToCentered`) and uses that, plus the Y offset from
   `UiCollectionSphereSetCellYOffset` (`cameraY`), as the camera's screen offset; then updates
   all its matrices and calls its virtual slot 2 (vtable `+0x10`). */

void UiCollectionSphereInitCameras(UiCollectionSphere *self)
{
  int i;
  GfxCamera *cam;
  const VtblEntry *slot;
  float pos[2];

  for (i = 0; i < 7; i++) {
    GfxCameraInit(self->cameras[i]);
    cam = self->cameras[i];
    cam->nearZ = 1.0f;
    cam->target[0] = 0.0f;
    cam->target[1] = 0.0f;
    cam->target[2] = 0.0f;
    cam->target[3] = 0.0f;
    cam->eye[0] = 0.0f;
    cam->eye[1] = 0.0f;
    cam->eye[2] = 0.0f;
    cam->eye[3] = 0.0f;
    UiScreenToCentered(self->cellPos[i][0], self->cellPos[i][1], pos);
    UiCollectionSphereSetCellYOffset(self, (u8)self->page, (u8)i);
    GfxCameraSetScreenOffset(pos[0], pos[1] + self->cameraY, self->cameras[i]);
    GfxCameraUpdate(self->cameras[i], 0xffffffff);
    cam = self->cameras[i];
    slot = &((const VtblEntry *)cam->base.vtable)[2];
    ((void (*)(void *))slot->fn)((u8 *)cam + slot->delta);
  }
}
