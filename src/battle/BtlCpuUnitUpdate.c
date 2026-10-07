// bdc 0x0889aa30 BtlCpuUnitUpdate
#include "bdc.h"

/* Per-frame update of the CPU-controlled battle Bakugan class (0x6d0 bytes, vtable `0x08af21b4`,
   built by `BtlCreateBakugan` for non-player modes): on the first frame (`aiStarted` clear)
   resolves the ally (`BtlCpuUnitResolveAlly`), applies `aiLevel` to the AI object
   (`BtlAiSetLevel`, low byte, skipped without an AI object) and sets `aiStarted`; then runs
   `BtlBakuganUpdate` and, when `BtlBakuganRunBallEntry` returns non-zero (no entry sequence
   pending) and the unit has an AI object, `BtlAiUpdate`. */

void BtlCpuUnitUpdate(BtlCpuUnit *unit)
{
  if (unit->aiStarted == 0) {
    BtlCpuUnitResolveAlly(unit);
    if (unit->ai != NULL) {
      BtlAiSetLevel(unit->ai, (u8)unit->aiLevel);
    }
    unit->aiStarted = 1;
  }
  BtlBakuganUpdate(&unit->base);
  if (BtlBakuganRunBallEntry(&unit->base) != 0 && unit->ai != NULL) {
    BtlAiUpdate(unit->ai);
  }
}
