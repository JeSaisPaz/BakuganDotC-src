// bdc 0x0890e284 UiConfirmDialogCtor
#include "bdc.h"

/* Constructor of the yes/no confirmation dialog screen (`UiConfirmDialog`), task id 510 (0x1fe)
   built by `CoreTaskNewById` (object size 0x94, base `UiScreenCtor`, vtable
   `g_uiConfirmDialogVtbl`). Allocates the 16-entry sprite table from the low heap, saves the pad's
   `stickEmulatesDpad` byte and forces it to 1, clears the cursor/alpha/pulse state, sets the
   dim-background and `unk92` flags, puts the active fader at sort key 5000, creates the text box
   `UiTextPrinterCreate(0)` with a 272-pixel wrap width, empties `g_confirmDialogMessage`, clears
   `g_uiConfirmDialogResult` and publishes itself in `g_uiConfirmDialogInstance`. Returns `task`. */

CoreTask *UiConfirmDialogCtor(CoreTask *task)
{
  UiConfirmDialog *self = (UiConfirmDialog *)task;
  bool fromLow;
  GfxSprite **sprites;
  PadState *pad;
  UiTextPrinter *printer;

  UiScreenCtor(task);
  task->vtable = &g_uiConfirmDialogVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(16 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  pad = self->pad;
  self->sprites = sprites;
  self->savedPadByte = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  self->unk24 = 0;
  self->unk70 = 0;
  self->subState = 0;
  self->cursor = 0;
  self->textAlpha = 0.0f;
  self->pulsePhase = 0.0f;
  self->unk84 = 0;
  self->unk91 = 1;
  self->unk92 = 1;
  GfxGetActiveFader()->sortKey = 5000.0f;
  printer = UiTextPrinterCreate(0);
  self->textPrinter = printer;
  printer->wrapWidth = 272.0f;
  strcpy(g_confirmDialogMessage, "");
  g_uiConfirmDialogResult = 0;
  g_uiConfirmDialogInstance = task;
  if (g_lastScreenTaskId == 302) {
    g_uiKeepSharedBg = 1;
  }
  self->unk70 = 1;
  return task;
}
