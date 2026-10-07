// bdc 0x0894893c UiBattleRecordLoadPhase
#include "bdc.h"

/* Phase 0 of `UiBattleRecord` (phase table `0x08a9d19c`): step 0 starts the
   `"main_bg.fab"` background (`UiSharedAnimStart`, depth 2.0, slot 0); step 1 primes the fader
   with 20000.0 and starts a 12-frame fade from `g_battleRecordFadeColorOpaque` to
   `g_battleRecordFadeColorClear`; step 2 creates the package node `package` if missing (MemAlloc
   from low memory + `IoLzsPackageCtor`; stays NULL if the allocation fails) and starts loading
   `"data/2d/<lang>/record.lzs"` (`IoLzsPackageStartLoad`), going to step 3 once the load is
   accepted; step 3 polls it (`IoLzsPackagePoll`) and, when done, switches to phase 3 (open),
   step 0. Other steps do nothing. */

void UiBattleRecordLoadPhase(UiBattleRecord *self)
{
  switch (self->base.phaseStep) {
  case 0:
    UiSharedAnimStart(2.0f, 0.0f, 0.0f, self, (void *)"main_bg.fab", 0, 0);
    self->base.phaseStep = 1;
    break;
  case 1:
    UiBattleRecordPrimeFader(20000.0f);
    UiBattleRecordStartFade(12.0f, g_battleRecordFadeColorOpaque, g_battleRecordFadeColorClear);
    self->base.phaseStep = 2;
    break;
  case 2: {
    char path[64];

    if (self->package == NULL) {
      IoLzsPackage *pkg = NULL;
      IoLzsPackage *mem;
      bool fromLow;

      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = MemAlloc(0x44, NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (mem != NULL) {
        IoLzsPackageCtor(mem);
        pkg = mem;
      }
      self->package = pkg;
    }
    path[0] = 0;
    memset(path + 1, 0, sizeof(path) - 1);
    SaveGetProfile();
    sprintf(path, "data/2d/%s/record.lzs", SaveGetLanguageDirName());
    if (IoLzsPackageStartLoad(self->package, path, 10, 1, 1) != 0) {
      self->base.phaseStep = 3;
    }
    break;
  }
  case 3:
    if (IoLzsPackagePoll(self->package, 1) != 0) {
      self->base.phase = 3;
      self->base.phaseStep = 0;
    }
    break;
  }
}
