// bdc 0x08809f60 UiLanguageSelectStateLoad
#include "bdc.h"

/* State 0 of the language-selection screen, by sub-step: 0 creates the package `pack` if missing
   (`IoLzsPackageCtor`) and starts loading `data/2d/%s/LanguageSetting.lzs` for the current
   language directory (`SaveGetLanguageDirName`, `IoLzsPackageStartLoad`), moving to sub-step 1
   once started; 1 polls it (`IoLzsPackagePoll`) and moves to sub-step 2 when done; any other
   sub-step switches to state 1 with sub-step 0. */

void UiLanguageSelectStateLoad(CoreTask *task)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  bool fromLow;
  IoLzsPackage *pack;
  IoLzsPackage *created;
  const char *dirName;
  char path[128];

  if (self->subStep == 0) {
    if (self->pack == (IoLzsPackage *)0x0) {
      created = (IoLzsPackage *)0x0;
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      pack = MemAlloc(0x44, (char *)0x0, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (pack != (IoLzsPackage *)0x0) {
        IoLzsPackageCtor(pack);
        created = pack;
      }
      self->pack = created;
    }
    dirName = SaveGetLanguageDirName();
    sprintf(path, "data/2d/%s/LanguageSetting.lzs", dirName);
    if (IoLzsPackageStartLoad(self->pack, path, 10, 1, 1) != 0) {
      self->subStep = 1;
    }
  }
  else if (self->subStep == 1) {
    if (self->pack != (IoLzsPackage *)0x0) {
      if (IoLzsPackagePoll(self->pack, 1) != 0) {
        self->subStep = 2;
      }
    }
  }
  else {
    self->state = 1;
    self->subStep = 0;
  }
  return;
}
