// bdc 0x0882ab20 GfxPlayerBlurTaskCtor
#include "bdc.h"

/* Constructor of the `GfxPlayerBlurTask` (task id 0x189, vtable `g_gfxPlayerBlurTaskVtbl`,
   created by `GameFieldPhaseLoad`): a full-screen afterimage/motion-blur post effect. When no
   instance is active (`g_playerBlurActive`) and `model` is given, it marks one active, stores
   `model`/`camera`, allocates a 0x20000-byte aligned capture buffer (`g_blurCaptureBuffer`,
   `MemAllocAligned`, zeroed) wrapped as the render-target texture "PlayerBlurTex"
   (`GfxTextureInitVramTarget`, 0x140-byte texture object from the low heap), clears alpha, the
   hold/frame counters and `g_playerBlurLastFrame`, sets `ownsObjects`, clamps the hold time
   `frames` to 180 and sets the light direction; otherwise it starts in the finished state 5.
   Inserts itself at priority 0 (`CoreTaskInsert`), sets id 0x189 and returns `task`. */

GfxPlayerBlurTask *GfxPlayerBlurTaskCtor(GfxPlayerBlurTask *task, GfxModel *model,
                                         GfxCamera *camera, s32 frames)
{
  bool fromLow;
  GfxTexture *obj;
  GfxTexture *tex;

  CoreTaskInit(&task->base);
  task->base.vtable = &g_gfxPlayerBlurTaskVtbl;
  if (g_playerBlurActive == 0 && model != NULL) {
    g_playerBlurActive = 1;
    task->model = model;
    task->camera = camera;
    task->buffer = MemAllocAligned(0x20000, true);
    g_blurCaptureBuffer = task->buffer;
    memset(task->buffer, 0, 0x20000);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    obj = (GfxTexture *)MemAlloc(0x140, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    tex = NULL;
    if (obj != NULL) {
      CoreObjectInit((CoreObject *)obj, NULL);
      obj->vtbl = g_gfxTextureVtbl;
      obj->unk11c = 0;
      tex = obj;
    }
    task->texture = tex;
    GfxTextureInitVramTarget(tex, "PlayerBlurTex", task->buffer);
    task->alpha = 0.0f;
    task->holdCounter = 0;
    task->frameCounter = 0;
    g_playerBlurLastFrame = 0;
    task->state = 0;
    task->compositeBuilt = 0;
    task->ownsObjects = 1;
    task->feedbackBuilt = 0;
    if (frames > 180) {
      frames = 180;
    }
    task->holdFrames = frames;
    task->lightDir[0] = 0.419f;
    task->lightDir[1] = 0.428f;
    task->lightDir[2] = -0.801f;
    task->lightDir[3] = 0.0f;
  }
  else {
    task->state = 5;
  }
  task->feedbackEnabled = 0;
  task->unk547 = 0;
  CoreTaskInsert(task, 0);
  task->base.id = 0x189;
  return task;
}
