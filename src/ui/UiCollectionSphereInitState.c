// bdc 0x0897903c UiCollectionSphereInitState
#include "bdc.h"

/* Class init of `UiCollectionSphere` called by `UiCollectionSphereCtor`:
   clears cursor/page/cursor-mode fields, builds the entry lists
   (`UiCollectionSphereBuildEntryList`), clears the bob/glow/model records, allocates seven
   0x2a0-byte cameras (`GfxCameraCtor`, `cameras[7]`, NULL when the allocation fails), clears
   the cell flags and the pop-out motion name, sets the entry count by category (0 -> 0x13,
   1 -> 0xb, 2 -> 0x10, others unchanged), clears the dim fades and the help printer block and
   creates the help printer (`UiCollectionSphereCreateHelpPrinter`). */

void UiCollectionSphereInitState(UiCollectionSphere *self)
{
  int i;

  self->cursor = 0;
  self->page = 0;
  self->oldPage = 0;
  self->pageDir = 0;
  self->cursorMode = 0;
  UiCollectionSphereBuildEntryList(self);
  memset(&self->bobOn, 0, 0xc);
  memset(&self->glowOn, 0, 0xc);
  memset(self->models, 0, sizeof(self->models));
  for (i = 0; i < 7; i++) {
    bool fromLow;
    CoreNode *cam;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    cam = MemAlloc(sizeof(GfxCamera), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (cam != NULL) {
      GfxCameraCtor(cam);
    }
    self->cameras[i] = cam;
  }
  memset(self->cellActive, 0, sizeof(self->cellActive));
  memset(self->motionModelName, 0, sizeof(self->motionModelName));
  switch (self->category) {
  case 0:
    self->entryCount = 0x13;
    break;
  case 1:
    self->entryCount = 0xb;
    break;
  case 2:
    self->entryCount = 0x10;
    break;
  }
  memset(self->dim, 0, sizeof(self->dim));
  memset(&self->helpPrinter, 0, 0x224);
  UiCollectionSphereCreateHelpPrinter(self);
}
