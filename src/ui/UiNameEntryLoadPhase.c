// bdc 0x08805138 UiNameEntryLoadPhase
#include "bdc.h"

/* Phase 0 of the name entry screen: loads `"data/2d/<lang>/name.lzs"` (`SaveGetLanguageDirName`)
   into `langPack` and `"data/name_common.lzs"` into `commonPack` (packages allocated from the low
   heap with `IoLzsPackageCtor` when missing, `IoLzsPackageStartLoad`/`IoLzsPackagePoll`),
   waits for both, then starts a 20-frame fade from black, switches to phase 1 and runs it at once
   through the Update virtual (vtable slot 2). */

void UiNameEntryLoadPhase(UiNameEntry *self)
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
    if (self->langPack == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      pkg = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (pkg != NULL) {
        IoLzsPackageCtor(pkg);
      }
      self->langPack = (CoreNode *)pkg;
    }
    path[0] = '\0';
    memset(&path[1], 0, sizeof(path) - 1);
    SaveGetProfile();
    sprintf(path, "data/2d/%s/name.lzs", SaveGetLanguageDirName());
    if (IoLzsPackageStartLoad((IoLzsPackage *)self->langPack, path, 10, 1, 1) != 0) {
      self->base.phaseStep = 2;
    }
    break;
  case 2:
    if (IoLzsPackagePoll((IoLzsPackage *)self->langPack, 1) != 0) {
      self->base.phaseStep = 3;
    }
    break;
  case 3:
    if (self->commonPack == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      pkg = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (pkg != NULL) {
        IoLzsPackageCtor(pkg);
      }
      self->commonPack = (CoreNode *)pkg;
    }
    if (IoLzsPackageStartLoad((IoLzsPackage *)self->commonPack, "data/name_common.lzs", 10, 1,
                              1) != 0) {
      self->base.phaseStep = 4;
    }
    break;
  case 4:
    if (IoLzsPackagePoll((IoLzsPackage *)self->commonPack, 1) != 0) {
      self->base.phaseStep = 5;
    }
    break;
  case 5:
    self->base.phaseStep = 6;
    /* fallthrough */
  case 6:
    /* fade from black over 20 frames */
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
    GfxFaderStart(GfxGetActiveFader(), 20);
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
