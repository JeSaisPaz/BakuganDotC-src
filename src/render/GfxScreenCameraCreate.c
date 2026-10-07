// bdc 0x089e3620 GfxScreenCameraCreate
#include "bdc.h"

/* Creates the 2D screen camera `g_gfxScreenCamera` (deleting an old one): a 0x2a0-byte camera
   (`GfxCameraCtor`) with an identity view matrix offset by (-0.02, -0.02) and the default
   orthographic projection (`GfxScreenCameraSetOrtho` with `g_gfxScreenOrthoRect`). Called by
   `GfxRenderInit`. */

void GfxScreenCameraCreate(void)

{
  bool fromLow;
  CoreNode *cam;
  CoreNode *newCam;

  if (g_gfxScreenCamera != (GfxCamera *)0x0) {
    const VtblEntry *dtor = &((const VtblEntry *)g_gfxScreenCamera->base.vtable)[1];
    ((void (*)(void *, int))dtor->fn)((u8 *)g_gfxScreenCamera + dtor->delta,3);
    g_gfxScreenCamera = (GfxCamera *)0x0;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  cam = MemAlloc(sizeof(GfxCamera),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  newCam = (CoreNode *)0x0;
  if (cam != (CoreNode *)0x0) {
    GfxCameraCtor(cam);
    newCam = cam;
  }
  g_gfxScreenCamera = (GfxCamera *)newCam;
  /* Identity view matrix (vmidt.q + sv.q), then a (-0.02, -0.02) translation. */
  g_gfxScreenCamera->view.x.x = 1.0f; g_gfxScreenCamera->view.x.y = 0.0f;
  g_gfxScreenCamera->view.x.z = 0.0f; g_gfxScreenCamera->view.x.w = 0.0f;
  g_gfxScreenCamera->view.y.x = 0.0f; g_gfxScreenCamera->view.y.y = 1.0f;
  g_gfxScreenCamera->view.y.z = 0.0f; g_gfxScreenCamera->view.y.w = 0.0f;
  g_gfxScreenCamera->view.z.x = 0.0f; g_gfxScreenCamera->view.z.y = 0.0f;
  g_gfxScreenCamera->view.z.z = 1.0f; g_gfxScreenCamera->view.z.w = 0.0f;
  g_gfxScreenCamera->view.w.x = 0.0f; g_gfxScreenCamera->view.w.y = 0.0f;
  g_gfxScreenCamera->view.w.z = 0.0f; g_gfxScreenCamera->view.w.w = 1.0f;
  g_gfxScreenCamera->view.w.y = -0.02f;
  g_gfxScreenCamera->view.w.x = -0.02f;
  GfxScreenCameraSetOrtho(g_gfxScreenOrthoRect);
}
