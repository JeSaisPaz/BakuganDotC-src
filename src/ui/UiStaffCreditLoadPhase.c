// bdc 0x089449f0 UiStaffCreditLoadPhase
#include "bdc.h"

/* Load phase (entry 0 of the phase table `0x08a9d078`) of the staff-credits screen (task 3004,
   `UiStaffCreditCtor`), driven by `phaseStep`:
   0 — allocates (low heap) the `IoLzsPackage` `langPackage` if missing and starts loading
       `"data/2d/%s/credit.lzs"` (language directory from `SaveGetLanguageDirName`); step 1 once started;
   1 — polls it, step 2 when loaded;
   2/3 — the same for `commonPackage` with `"data/credit_common.lzs"`;
   4 — primes the fader (sort key 20000) and starts a 16-frame fade from `g_staffCreditFadeBlack` to
       `g_staffCreditFadeClear`, creates the shared text renderer if it does not exist (setting its
       packet depth to 2000), then moves on to phase 1 step 0.
   Steps >= 5 do nothing. */

void UiStaffCreditLoadPhase(UiScreen *screen)
{
  UiStaffCredit *self = (UiStaffCredit *)screen;
  char path[64];
  UiTextBox *box;

  switch ((u32)screen->phaseStep) {
  case 0:
    if (self->langPackage == NULL) {
      IoLzsPackage *pkg = NULL;
      IoLzsPackage *mem;
      bool fromLow;

      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (mem != NULL) {
        IoLzsPackageCtor(mem);
        pkg = mem;
      }
      self->langPackage = pkg;
    }
    path[0] = '\0';
    memset(&path[1], 0, sizeof(path) - 1);
    SaveGetProfile();
    sprintf(path, "data/2d/%s/credit.lzs", SaveGetLanguageDirName());
    if (IoLzsPackageStartLoad(self->langPackage, path, 10, 1, 1) != 0) {
      screen->phaseStep = 1;
    }
    break;
  case 1:
    if (IoLzsPackagePoll(self->langPackage, 1) != 0) {
      screen->phaseStep = 2;
    }
    break;
  case 2:
    if (self->commonPackage == NULL) {
      IoLzsPackage *pkg = NULL;
      IoLzsPackage *mem;
      bool fromLow;

      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (mem != NULL) {
        IoLzsPackageCtor(mem);
        pkg = mem;
      }
      self->commonPackage = pkg;
    }
    if (IoLzsPackageStartLoad(self->commonPackage, "data/credit_common.lzs", 10, 1, 1) != 0) {
      screen->phaseStep = 3;
    }
    break;
  case 3:
    if (IoLzsPackagePoll(self->commonPackage, 1) != 0) {
      screen->phaseStep = 4;
    }
    break;
  case 4:
    UiStaffCreditPrimeFader(20000.0f);
    UiStaffCreditStartFade(16.0f, g_staffCreditFadeBlack, g_staffCreditFadeClear);
    if (!UiTextRenderExists()) {
      UiTextRenderEnsure();
      box = UiTextRenderGetBox();
      box->packetDepth = 2000.0f;
    }
    screen->phaseStep = 0;
    screen->phase = 1;
    break;
  default:
    break;
  }
}
