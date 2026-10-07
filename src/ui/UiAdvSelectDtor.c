// bdc 0x08917970 UiAdvSelectDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the adventure character/Bakugan select screen (task id 376):
   restores `g_uiAdvSelectVtbl`, waits for the GE, sets the frame mode to 1, restores the pad's
   stick-emulates-d-pad byte, frees the models (`UiAdvSelectFreeModels`) and all motions, deletes the camera
   `+0x910` (virtual dtor, flag 3), stores its id in `g_lastScreenTaskId`. Then
   `UiScreenDtor``(screen, 0)`; frees the object when `flags & 1`. */

typedef struct AdvSelectCamVSlot {
  s16 adjust;
  s16 pad;
  void (*fn)(void *, s32);
} AdvSelectCamVSlot;

typedef struct AdvSelectCamVTable {
  u8 unk00[8];
  AdvSelectCamVSlot dtor;
} AdvSelectCamVTable;

typedef struct AdvSelectCamObj {
  u8 unk00[0x20];
  AdvSelectCamVTable *vtable;
} AdvSelectCamObj;

void UiAdvSelectDtor(UiAdvSelect *self, u32 flags)
{
  if (self != NULL) {
    (self->base).base.vtable = g_uiAdvSelectVtbl;
    GfxWaitGeIdle();
    UiScreenSetFrameMode((CoreTask *)self, 1);
    (self->base).pad->stickEmulatesDpad = self->savedStickEmulatesDpad;
    UiAdvSelectFreeModels(self);
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
    if (self->camera != NULL) {
      AdvSelectCamObj *cam = (AdvSelectCamObj *)self->camera;
      AdvSelectCamVSlot *slot = &cam->vtable->dtor;

      slot->fn((u8 *)cam + slot->adjust, 3);
      self->camera = NULL;
    }
    g_lastScreenTaskId = (self->base).base.id;
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, NULL, 0);
      MemUnlock();
    }
  }
}
