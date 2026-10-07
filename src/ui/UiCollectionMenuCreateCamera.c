// bdc 0x08973fa4 UiCollectionMenuCreateCamera
#include "bdc.h"

/* Creates the 3D camera of `UiCollectionMenu` (`GfxSetActiveCamera`,
   stored at `+0x518`): target at the origin, eye at z = 200, then updates it (`GfxCameraUpdate`)
   and calls its virtual method at vtable `+0x10` (slot 2). */

void UiCollectionMenuCreateCamera(UiCollectionMenu *self)

{
  GfxCamera *cam = (GfxCamera *)GfxSetActiveCamera((void *)0);
  const VtblEntry *update;

  self->camera = cam;
  cam->target[0] = 0.0f;
  cam->target[1] = 0.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam = self->camera;
  cam->eye[0] = 0.0f;
  cam->eye[1] = 0.0f;
  cam->eye[2] = 200.0f;
  cam->eye[3] = 0.0f;
  GfxCameraUpdate(self->camera, 0xffffffff);
  update = &((const VtblEntry *)self->camera->base.vtable)[2];
  ((void (*)(void *))update->fn)((u8 *)self->camera + update->delta);
  return;
}
