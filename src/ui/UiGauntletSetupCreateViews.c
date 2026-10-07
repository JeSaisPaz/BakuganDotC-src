// bdc 0x08932c94 UiGauntletSetupCreateViews
#include "bdc.h"

/* Creates the two 3D views (cameras) of the gauntlet setup screen (`UiGauntletSetup`):
   allocates (low heap, `MemSetAllocFromLow`) an array of two 0x2a0-byte `GfxCamera` objects
   (`CxxVecNew` with constructor `GfxCameraCtor` after the `g_cxxVecCookieSize` cookie) into `views`
   (NULL when the allocation fails), then for each view: `GfxCameraInit`, sets eye/target and the screen
   offset (`GfxCameraSetScreenOffset`) — view 0: target (0,0,0), eye (0,0,150), offset (-140,-90);
   view 1: target (0,140,0), eye (0,140,150), offset (0,-80), near 90 / far 350 — recomputes all matrices
   (`GfxCameraUpdate` with flags -1) and calls the camera's vtable slot 2 (+0x10). */

void UiGauntletSetupCreateViews(UiGauntletSetup *self)
{
  bool fromLow;
  void *block;
  GfxCamera *views;
  GfxCamera *cam;
  const VtblEntry *slot;
  s32 i;

  views = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(0x550, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (block != NULL) {
    views = CxxVecNew((u8 *)block + g_cxxVecCookieSize, 2, sizeof(GfxCamera), GfxCameraCtor, 0);
  }
  self->views = views;
  i = 0;
  do {
    GfxCameraInit(&((GfxCamera *)self->views)[i]);
    cam = &((GfxCamera *)self->views)[i];
    if (i == 0) {
      cam->target[0] = 0.0f;
      cam->target[1] = 0.0f;
      cam->target[2] = 0.0f;
      cam->target[3] = 0.0f;
      cam = &((GfxCamera *)self->views)[i];
      cam->eye[0] = 0.0f;
      cam->eye[1] = 0.0f;
      cam->eye[2] = 150.0f;
      cam->eye[3] = 0.0f;
      GfxCameraSetScreenOffset(-140.0f, -90.0f, &((GfxCamera *)self->views)[i]);
    } else {
      cam->target[0] = 0.0f;
      cam->target[1] = 140.0f;
      cam->target[2] = 0.0f;
      cam->target[3] = 0.0f;
      cam = &((GfxCamera *)self->views)[i];
      cam->eye[0] = 0.0f;
      cam->eye[1] = 140.0f;
      cam->eye[2] = 150.0f;
      cam->eye[3] = 0.0f;
      GfxCameraSetScreenOffset(0.0f, -80.0f, &((GfxCamera *)self->views)[i]);
      ((GfxCamera *)self->views)[i].nearZ = 90.0f;
      ((GfxCamera *)self->views)[i].farZ = 350.0f;
    }
    GfxCameraUpdate(&((GfxCamera *)self->views)[i], 0xffffffff);
    cam = &((GfxCamera *)self->views)[i];
    slot = &((const VtblEntry *)cam->base.vtable)[2];
    ((void (*)(void *))slot->fn)((u8 *)cam + slot->delta);
    i++;
  } while (i < 2);
}
