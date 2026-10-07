// bdc 0x089939e4 UiUnlockCodeLoadPhase
#include "bdc.h"

/* Phase 0 of `UiUnlockCode`: loads the localized `"data/2d/%s/name.lzs"` package
   (`+0x6c`) and `"data/name_common.lzs"` (`+0x70`) through
   `IoLzsPackageStartLoad`/`IoLzsPackagePoll` (packages allocated from the low heap with
   `IoLzsPackageCtor`), then starts an 8-frame fade from black, switches to phase 1 and runs it at
   once through the Update virtual (vtable slot 2). */

void UiUnlockCodeLoadPhase(UiUnlockCode *self)
{
  bool fromLow;
  IoLzsPackage *pkg;
  GfxFader *fader;
  const VtblEntry *entry;
  char path[64];

  switch (self->base.phaseStep) {
  case 0:
    self->base.phaseStep = 1;
    /* fallthrough */
  case 1:
    if (self->package == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      pkg = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (pkg != NULL) {
        IoLzsPackageCtor(pkg);
      }
      self->package = pkg;
    }
    path[0] = '\0';
    memset(&path[1], 0, sizeof(path) - 1);
    SaveGetProfile();
    sprintf(path, "data/2d/%s/name.lzs", SaveGetLanguageDirName());
    if (IoLzsPackageStartLoad(self->package, path, 10, 1, 1) != 0) {
      self->base.phaseStep = 2;
    }
    break;
  case 2:
    if (IoLzsPackagePoll(self->package, 1) != 0) {
      self->base.phaseStep = 3;
    }
    break;
  case 3:
    if (self->commonPackage == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      pkg = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (pkg != NULL) {
        IoLzsPackageCtor(pkg);
      }
      self->commonPackage = pkg;
    }
    if (IoLzsPackageStartLoad(self->commonPackage, "data/name_common.lzs", 10, 1, 1) != 0) {
      self->base.phaseStep = 4;
    }
    break;
  case 4:
    if (IoLzsPackagePoll(self->commonPackage, 1) != 0) {
      self->base.phaseStep = 5;
    }
    break;
  case 5:
    self->base.phaseStep = 6;
    /* fallthrough */
  case 6:
    /* fade from black over 8 frames */
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 1.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    GfxFaderStart(GfxGetActiveFader(), 8);
    entry = &((const VtblEntry *)self->base.base.vtable)[2];
    self->base.phaseStep = 7;
    goto next_phase;
  case 7:
    entry = &((const VtblEntry *)self->base.base.vtable)[2];
  next_phase:
    /* switch to phase 1 and run it at once through the Update virtual (slot 2) */
    self->base.phase = 1;
    self->base.phaseStep = 0;
    ((void (*)(void *))entry->fn)((char *)self + entry->delta);
    break;
  }
}
