// bdc 0x088b99cc GameFieldCameraDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the field camera (`GameFieldCameraCtor`): restores
   `g_gameFieldCameraVtbl`, deletes the quest camera controller `+0x5c4` and its parameter object
   `+0x5c8` (`GameQuestCamParams`) through their virtual deleting destructors (flag 3) and clears
   them, calls `GameQuestPathSetUnload`/`GameQuestCamTableUnload` (quest path/camera table release),
   destroys the mode helpers `+0x400`, `+0x3d0`, `+0x3c0` (flag 2), runs the base camera destructor
   `GfxCameraDtor` and frees the object when `flags & 1`. Does nothing for a NULL `cam`. */

void GameFieldCameraDtor(GameFieldCamera *cam, u32 flags)
{
  GameQuestCamCtrl *ctrl;
  GameQuestCamParams *params;
  const VtblEntry *dtor;

  if (cam != NULL) {
    cam->base.base.vtable = g_gameFieldCameraVtbl;
    GameFieldCameraDtorHook(cam);
    ctrl = cam->questCam;
    if (ctrl != NULL) {
      dtor = &ctrl->vtbl[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)ctrl + dtor->delta, 3);
      cam->questCam = NULL;
    }
    params = cam->questCamParams;
    if (params != NULL) {
      dtor = &params->vtbl[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)params + dtor->delta, 3);
      cam->questCamParams = NULL;
    }
    GameQuestPathSetUnload();
    GameQuestCamTableUnload();
    GameFieldCameraAimViewDtor(cam->aimView, 2);
    GameFieldCameraHeadViewDtor(cam->headView, 2);
    GameFieldCameraMode9Dtor((GameFieldCameraMode9State **)cam->mode9, 2);
    GfxCameraDtor(&cam->base.base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(cam, NULL, 0);
      MemUnlock();
    }
  }
}
