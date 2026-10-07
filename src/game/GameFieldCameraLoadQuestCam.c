// bdc 0x088bccd8 GameFieldCameraLoadQuestCam
#include "bdc.h"

/* Loads the per-quest camera data of the field camera (`GameFieldCameraCtor`): looks up
   `"f%d_quest_%02d_00.ptlb"` (path) in the pack chain `g_ioLzsPackages` with the current field and
   quest numbers `g_gameEventFlags[0]`/`g_gameEventFlags[2]` and returns early when it is missing;
   otherwise parses it (`GameQuestPathSetLoad` on `{data, size}`), then does the same for
   `"f%d_quest_%02d_00.cptb"` (camera points, `GameQuestCamTableLoad` on `{data, size, 50.0}`).
   With both present it allocates the parameter object `+0x5c8` (`GameQuestCamParams`, 0x20 bytes,
   id -1, vtable `g_gameFieldCamParamVtbl`) and the quest camera controller `+0x5c4`
   (`GameQuestCamCtrlCtor`, 0x50 bytes, from a `GameQuestCamCtrlDesc`), sets fov `+0x158 = 45`
   and primes the spring (`GameFieldCameraUpdateQuestCam`).
   The descriptor's `eye` and `lookAt` and the parameter object's `target` are zero vectors (bank
   constant C720). */

void GameFieldCameraLoadQuestCam(GameFieldCamera *cam)
{
  struct {
    void *data;
    u32 size;
  } pathFile;
  struct {
    void *data;
    u32 size;
    float f8;
  } camFile;
  GameQuestCamCtrlDesc desc;
  char name[64];
  bool fromLow;
  GameQuestCamParams *params;
  GameQuestCamParams *paramsResult;
  GameQuestCamCtrl *ctrl;
  GameQuestCamCtrl *ctrlResult;

  pathFile.data = NULL;
  pathFile.size = 0;
  sprintf(name, "f%d_quest_%02d_00.ptlb", g_gameEventFlags[0], g_gameEventFlags[2]);
  pathFile.data = CorePackChainFind(g_ioLzsPackages, name);
  if (pathFile.data == NULL) {
    return;
  }
  pathFile.size = CorePackChainFindSize(g_ioLzsPackages, name);
  GameQuestPathSetLoad((void **)&pathFile);
  camFile.data = NULL;
  camFile.size = 0;
  camFile.f8 = 50.0f;
  sprintf(name, "f%d_quest_%02d_00.cptb", g_gameEventFlags[0], g_gameEventFlags[2]);
  camFile.data = CorePackChainFind(g_ioLzsPackages, name);
  if (camFile.data == NULL) {
    return;
  }
  camFile.size = CorePackChainFindSize(g_ioLzsPackages, name);
  GameQuestCamTableLoad(&camFile);
  desc.eye.x = 0.0f;
  desc.eye.y = 0.0f;
  desc.eye.z = 0.0f;
  desc.eye.w = 0.0f;
  desc.lookAt.x = 0.0f;
  desc.lookAt.y = 0.0f;
  desc.lookAt.z = 0.0f;
  desc.lookAt.w = 0.0f;
  desc.extra[0] = 3.9f;
  desc.extra[1] = 30000.0f;
  desc.params = NULL;
  desc.collision = NULL;
  desc.f38 = 0.3f;
  desc.f3c = 1.2f;
  desc.f40 = 600.0f;
  desc.radius = 200.0f;
  paramsResult = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  params = MemAlloc(sizeof(GameQuestCamParams), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (params != NULL) {
    params->vtbl = (const VtblEntry *)g_gameFieldCamParamBaseVtbl;
    params->id = -1;
    params->vtbl = (const VtblEntry *)g_gameFieldCamParamVtbl;
    params->target[0] = 0.0f;
    params->target[1] = 0.0f;
    params->target[2] = 0.0f;
    params->target[3] = 0.0f;
    paramsResult = params;
  }
  cam->questCamParams = paramsResult;
  desc.params = paramsResult;
  ctrlResult = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  ctrl = MemAlloc(sizeof(GameQuestCamCtrl), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (ctrl != NULL) {
    GameQuestCamCtrlCtor(ctrl, &desc);
    ctrlResult = ctrl;
  }
  cam->questCam = ctrlResult;
  cam->base.fov = 45.0f;
  GameFieldCameraUpdateQuestCam(cam);
}
