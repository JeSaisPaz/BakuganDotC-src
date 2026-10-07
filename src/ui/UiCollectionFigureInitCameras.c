// bdc 0x0898c078 UiCollectionFigureInitCameras
#include "bdc.h"

/* Initialises the six cell cameras of `UiCollectionFigure` (`+0x11f0[i]`,
   `GfxCameraInit`): resets them, converts each cell anchor (`+0x1228`) to world space
   (`UiScreenToCentered`), places the camera there and updates it. */

void UiCollectionFigureInitCameras(UiCollectionFigure *self)

{
  int i;
  float pos[2];

  for (i = 0; i < 6; i++) {
    GfxCamera *cam = (GfxCamera *)self->cameras[i];
    const VtblEntry *update;

    GfxCameraInit(cam);
    cam->nearZ = 1.0f;
    cam = (GfxCamera *)self->cameras[i];
    cam->target[0] = 0.0f;
    cam->target[1] = 0.0f;
    cam->target[2] = 0.0f;
    cam->target[3] = 0.0f;
    cam = (GfxCamera *)self->cameras[i];
    cam->eye[0] = 0.0f;
    cam->eye[1] = 0.0f;
    cam->eye[2] = 0.0f;
    cam->eye[3] = 0.0f;
    UiScreenToCentered(self->cellAnchor[i][0], self->cellAnchor[i][1], pos);
    GfxCameraSetScreenOffset(pos[0], pos[1], (GfxCamera *)self->cameras[i]);
    GfxCameraUpdate((GfxCamera *)self->cameras[i], 0xffffffff);
    cam = (GfxCamera *)self->cameras[i];
    update = &((const VtblEntry *)cam->base.vtable)[2];
    ((void (*)(void *))update->fn)((u8 *)cam + update->delta);
  }
}
