// bdc 0x08918268 UiAdvSelectCreateCamera
#include "bdc.h"

/* Creates the 3D camera of the adventure partner-select screen (`UiAdvSelectCtor`, task 376)
   with `GfxSetActiveCamera(0)`, stored at `+0x910`: target at the origin, eye (0, 0, 150), screen
   offset (-120, 0) via `GfxCameraSetScreenOffset`, full matrix update (`GfxCameraUpdate`), then
   calls virtual slot at camera vtable `+0x10` (the update hook). */

void UiAdvSelectCreateCamera(UiAdvSelect *self)
{
  GfxCamera *cam;
  const VtblEntry *slot;

  cam = (GfxCamera *)GfxSetActiveCamera(NULL);
  self->camera = cam;
  cam->target[0] = 0.0f;
  cam->target[1] = 0.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam = (GfxCamera *)self->camera;
  cam->eye[0] = 0.0f;
  cam->eye[1] = 0.0f;
  cam->eye[2] = 150.0f;
  cam->eye[3] = 0.0f;
  GfxCameraSetScreenOffset(-120.0f, 0.0f, (GfxCamera *)self->camera);
  GfxCameraUpdate((GfxCamera *)self->camera, 0xffffffff);
  cam = (GfxCamera *)self->camera;
  slot = &((const VtblEntry *)cam->base.vtable)[2];
  ((void (*)(void *))slot->fn)((u8 *)cam + slot->delta);
}
