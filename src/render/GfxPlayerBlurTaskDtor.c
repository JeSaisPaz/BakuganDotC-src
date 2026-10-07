// bdc 0x0882acf8 GfxPlayerBlurTaskDtor
#include "bdc.h"

/* Destructor of the `GfxPlayerBlurTask` (vtable slot 1): restores `g_gfxPlayerBlurTaskVtbl`;
   unless it never started (state 5) it waits for the GE while still drawing (state below 4,
   `GfxWaitGeIdle`), deletes the blur texture (virtual dtor, flag 3), clears the display's signal
   hook (so `GfxCopyFrameToBuffer` stops), frees the buffer (`MemFreeAligned`), deletes and
   clears the model and camera when it owns them, and clears `g_playerBlurActive`; then
   `CoreTaskDestroy` and, when `flags & 1`, frees the task. */

void GfxPlayerBlurTaskDtor(GfxPlayerBlurTask *task, u32 flags)
{
  if (task != NULL) {
    task->base.vtable = &g_gfxPlayerBlurTaskVtbl;
    if (task->state != 5) {
      if (task->state < 4) {
        GfxWaitGeIdle();
      }
      if (task->texture != NULL) {
        GfxTexture *tex = task->texture;
        const VtblEntry *dtor = &tex->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)tex + dtor->delta, 3);
      }
      g_gfxDisplay->signalHook = NULL;
      MemFreeAligned(task->buffer);
      if (task->ownsObjects != 0) {
        if (task->model != NULL) {
          GfxModel *model = task->model;
          const VtblEntry *dtor = &((const VtblEntry *)model->base.vtable)[1];
          ((void (*)(void *, s32))dtor->fn)((u8 *)model + dtor->delta, 3);
          task->model = NULL;
        }
        if (task->camera != NULL) {
          GfxCamera *camera = task->camera;
          const VtblEntry *dtor = &((const VtblEntry *)camera->base.vtable)[1];
          ((void (*)(void *, s32))dtor->fn)((u8 *)camera + dtor->delta, 3);
          task->camera = NULL;
        }
      }
      g_playerBlurActive = 0;
    }
    CoreTaskDestroy(&task->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
