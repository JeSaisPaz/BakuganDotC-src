// bdc 0x0890e3b8 UiConfirmDialogDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the yes/no confirm dialog (task id 510): restores the pad byte
   `+0x3c`, resets the fader z, destroys the text box at `+0x74`, clears the instance pointer
   `0x08ac108c`, runs the UiScreen base destructor `UiScreenDtor` and frees the object when `flags
   & 1`. */

void UiConfirmDialogDtor(CoreTask *task, u32 flags)

{
  UiConfirmDialog *self = (UiConfirmDialog *)task;
  UiScreen *screen = (UiScreen *)task;
  GfxFader *fader;
  UiTextPrinter *printer;

  if (task != (CoreTask *)0x0) {
    task->vtable = &g_uiConfirmDialogVtbl;
    GfxWaitGeIdle();
    if (screen->pad != (PadState *)0x0) {
      screen->pad->stickEmulatesDpad = self->savedPadByte;
    }
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
    printer = self->textPrinter;
    if (printer != (UiTextPrinter *)0x0) {
      const VtblEntry *e = (const VtblEntry *)printer->layer.vtbl + 1;
      ((void (*)(void *, int))e->fn)((u8 *)printer + e->delta, 3);
      self->textPrinter = (UiTextPrinter *)0x0;
    }
    g_uiConfirmDialogInstance = (CoreTask *)0x0;
    UiScreenDtor(screen, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
