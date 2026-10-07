// bdc 0x08913044 UiUpgradeGetUpgradeId
#include "bdc.h"

/* Returns the upgrade id of slot `slot` (0..5) for Bakugan `bakugan`:
   `g_upgradeSlotIds[g_upgradeClassTable[bakugan] * 6 + slot]` (6 slots per Bakugan class); 0 for other slots.
   `bakugan` is a Bakugan id 0..20 (the game passes 1..20), unguarded like the original. */

u8 UiUpgradeGetUpgradeId(int bakugan, int slot)

{
  if ((-1 < slot) && (slot < 6)) {
    return g_upgradeSlotIds[g_upgradeClassTable[bakugan] * 6 + slot];
  }
  return '\0';
}
