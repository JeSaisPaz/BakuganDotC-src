// bdc 0x089b3100 UiComboListLoadPhase
#include "bdc.h"

/* Phase 0 of the combo list screen. Step 0: takes the combo id from the player's Bakugan
   (`BtlGetPlayerBakugan`, returns early when there is none), builds
   `"data/2d/<lang>/combolist/<page>.lzs"` from `g_comboListPageNames``[combo - 1]`, allocates
   the `IoLzsPackage` `pack` from low memory on first use and starts its load
   (`IoLzsPackageStartLoad`), moving to step 1 once accepted. Step 1: when `IoLzsPackagePoll`
   reports the package loaded, resets `step` and advances the screen phase. */

void UiComboListLoadPhase(UiComboList *self)
{
  char path[128];
  BtlBakugan *unit;
  IoLzsPackage *pack;
  bool fromLow;

  if (self->step > 0) {
    if (self->step < 2 && IoLzsPackagePoll((IoLzsPackage *)self->pack, 1) != 0) {
      self->step = 0;
      self->base.phase++;
    }
    return;
  }
  if (self->step < 0)
    return;

  unit = (BtlBakugan *)BtlGetPlayerBakugan();
  if (unit == NULL)
    return;
  self->combo = (int)unit->base.base.unk08;
  SaveGetProfile();
  sprintf(path, "data/2d/%s/combolist/%s.lzs", SaveGetLanguageDirName(),
          g_comboListPageNames[self->combo - 1]);
  if (self->pack == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    pack = (IoLzsPackage *)MemAlloc(sizeof(IoLzsPackage), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (pack != NULL) {
      IoLzsPackageCtor(pack);
    }
    self->pack = pack != NULL ? &pack->base : NULL;
  }
  if (IoLzsPackageStartLoad((IoLzsPackage *)self->pack, path, 10, 1, 1) != 0) {
    self->step++;
  }
}
