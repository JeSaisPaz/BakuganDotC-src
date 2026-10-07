// bdc 0x08939770 UiUnlockResultCreateCamera
#include "bdc.h"

/* Creates the camera of `UiUnlockResult` for the reward model: allocates a
   0x2a0-byte camera from the low heap (`MemAlloc`), constructs it (`GfxCameraCtor`; NULL when
   the allocation failed) into `camera`, resets it (`GfxCameraInit`), sets `nearZ` 1 and the target
   to the origin. Reward kind 6: eye (0, 15, 55), screen offset (10, 20)
   (`GfxCameraSetScreenOffset`); kind 5: eye (0, 0, 25) for `g_rewardMaxusSet` 0, (0, 0, 20) for
   set 1 (unchanged otherwise), screen offset (0, -10). Then a full `GfxCameraUpdate` and the
   camera's virtual update hook (vtable slot 2). */

void UiUnlockResultCreateCamera(UiUnlockResult *self)

{
  bool fromLow;
  GfxCamera *cam;
  const VtblEntry *update;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  cam = (GfxCamera *)MemAlloc(sizeof(GfxCamera), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (cam != NULL) {
    GfxCameraCtor(&cam->base);
  }
  self->camera = cam;
  GfxCameraInit(cam);
  ((GfxCamera *)self->camera)->nearZ = 1.0f;
  cam = (GfxCamera *)self->camera;
  cam->target[0] = 0.0f;
  cam->target[1] = 0.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam = (GfxCamera *)self->camera;
  if (self->rewardKind == 6) {
    cam->eye[0] = 0.0f;
    cam->eye[1] = 15.0f;
    cam->eye[2] = 55.0f;
    cam->eye[3] = 0.0f;
    GfxCameraSetScreenOffset(10.0f, 20.0f, (GfxCamera *)self->camera);
  }
  else if (self->rewardKind == 5) {
    if (g_rewardMaxusSet == 0) {
      cam->eye[0] = 0.0f;
      cam->eye[1] = 0.0f;
      cam->eye[3] = 0.0f;
      cam->eye[2] = 25.0f;
      cam = (GfxCamera *)self->camera;
    }
    else if (g_rewardMaxusSet == 1) {
      cam->eye[0] = 0.0f;
      cam->eye[1] = 0.0f;
      cam->eye[3] = 0.0f;
      cam->eye[2] = 20.0f;
      cam = (GfxCamera *)self->camera;
    }
    GfxCameraSetScreenOffset(0.0f, -10.0f, cam);
  }
  GfxCameraUpdate((GfxCamera *)self->camera, 0xffffffff);
  update = &((const VtblEntry *)((GfxCamera *)self->camera)->base.vtable)[2];
  ((void (*)(void *))update->fn)((u8 *)self->camera + update->delta);
}
