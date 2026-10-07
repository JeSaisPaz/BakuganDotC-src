// bdc 0x0892cc78 UiBakuganSelectCreateCamera
#include "bdc.h"

/* Creates the 3D camera `+0x1cf4` of the Bakugan select screen (`UiBakuganSelectCtor`, task 371;
   cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries)
   (`GfxSetActiveCamera(0)`): target at the origin, eye (0, 0, 150),
   `GfxCameraSetScreenOffset(-40)`, full update (`GfxCameraUpdate`). */

void UiBakuganSelectCreateCamera(UiBakuganSelect *self)

{
  GfxCamera *cam = (GfxCamera *)GfxSetActiveCamera((void *)0);
  const VtblEntry *update;

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
  GfxCameraSetScreenOffset(-40.0f, 0.0f, (GfxCamera *)self->camera);
  GfxCameraUpdate((GfxCamera *)self->camera, 0xffffffff);
  update = &((const VtblEntry *)((GfxCamera *)self->camera)->base.vtable)[2];
  ((void (*)(void *))update->fn)((u8 *)self->camera + update->delta);
}
