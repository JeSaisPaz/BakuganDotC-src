// bdc 0x0898b0b0 UiCollectionFigureInitState
#include "bdc.h"

/* Class init of `UiCollectionFigure` called by `UiCollectionFigureCtor`:
   clears cursor/page/cursor-mode fields, builds the entry lists
   (`UiCollectionFigureBuildEntryList`), clears the tint/spin/model records, allocates six
   0x2a0-byte cell cameras (`GfxCameraCtor`, `cameras[6]`, NULL when the allocation fails),
   clears the slow-spin flags, sets the entry count by category (0 -> 0x12, 1 -> 2, others
   unchanged), clears the dim fades and the help printer block, creates the help printer
   (`UiCollectionFigureCreateHelpPrinter`) and sets the model base scale by category
   (0 -> 0.08, 1 -> 0.06, others unchanged). */

void UiCollectionFigureInitState(UiCollectionFigure *self)
{
  int i;

  self->cursor = 0;
  self->page = 0;
  self->targetPage = 0;
  self->pageDir = 0;
  self->cursorMode = 0;
  UiCollectionFigureBuildEntryList(self);
  memset(&self->tintOn, 0, 0xc);
  memset(&self->spinOn, 0, 0xc);
  memset(self->models, 0, sizeof(self->models));
  for (i = 0; i < 6; i++) {
    bool fromLow;
    CoreNode *cam;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    cam = MemAlloc(0x2a0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (cam != NULL) {
      GfxCameraCtor(cam);
    }
    self->cameras[i] = cam;
  }
  memset(self->slowSpin, 0, sizeof(self->slowSpin));
  switch (self->category) {
  case 0:
    self->entryCount = 0x12;
    break;
  case 1:
    self->entryCount = 2;
    break;
  }
  memset(self->dim, 0, sizeof(self->dim));
  memset(&self->helpPrinter, 0, 0x224);
  UiCollectionFigureCreateHelpPrinter(self);
  switch (self->category) {
  case 0:
    self->baseScale = 0.08f;
    break;
  case 1:
    self->baseScale = 0.06f;
    break;
  }
}
