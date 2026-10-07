// bdc 0x08910258 UiPauseDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the pause menu (task id 410): reinstalls `g_uiPauseVtbl`, waits
   for the GE. When a NetPlay manager exists and `base.netPad` is set, deletes that remote pad
   through its virtual destructor (flag 3), clears it, clears the manager's remote pad
   stick-emulates-d-pad byte and points `base.pad` back at `g_padState`. Then always restores the
   pad's stick-emulates-d-pad byte from `savedStickEmulatesDpad`, records the task id in
   `g_lastScreenTaskId`, resets the frame mode to 1, deletes the two hint text printers (flag 3),
   stores the task id in profile word 0x1d (`SaveProfileSetWord`) and calls
   `UiHelpLineDestroy`. Finally `UiScreenDtor``(screen, 0)`; frees the object when `flags & 1`. */

void UiPauseDtor(UiPause *self, u32 flags)

{
  PadState *netPad;
  const VtblEntry *entry;
  UiTextPrinter **printers;
  s32 i;

  if (self != (UiPause *)0x0) {
    self->base.base.vtable = g_uiPauseVtbl;
    GfxWaitGeIdle();
    if (NetPlayHasManager()) {
      netPad = self->base.netPad;
      if (netPad != (PadState *)0x0) {
        entry = &((const VtblEntry *)netPad->vtable)[1];
        ((void (*)(void *, s32))entry->fn)((u8 *)netPad + entry->delta, 3);
        self->base.netPad = (PadState *)0x0;
        ((NetPlay *)NetPlayGetManager())->remotePad->stickEmulatesDpad = 0;
        self->base.pad = g_padState;
      }
    }
    self->base.pad->stickEmulatesDpad = self->savedStickEmulatesDpad;
    g_lastScreenTaskId = self->base.base.id;
    UiScreenSetFrameMode((CoreTask *)self, 1);
    printers = self->hintPrinters;
    for (i = 0; i < 2; i++) {
      if (printers[i] != (UiTextPrinter *)0x0) {
        entry = &printers[i]->layer.vtbl[1];
        ((void (*)(void *, s32))entry->fn)((u8 *)printers[i] + entry->delta, 3);
        printers[i] = (UiTextPrinter *)0x0;
      }
    }
    SaveProfileSetWord(SaveGetProfile(), 0x1d, self->base.base.id);
    UiHelpLineDestroy();
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
