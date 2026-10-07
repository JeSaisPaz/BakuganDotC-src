// bdc 0x0890f9f4 UiRepairKindMessageIndex
#include "bdc.h"

/* Returns the message line index in `DMMesRepair_eu.bin` for repair kind `kind`
   (`UiRepairSetupPhase`): 2 → 1, 3 → 10, otherwise 0. */

int UiRepairKindMessageIndex(void *screen, int kind)

{
  if (kind < 3) {
    if (kind >= 2) {
      return 1;
    }
    return 0;
  }
  if (kind < 4) {
    return 10;
  }
  return 0;
}
