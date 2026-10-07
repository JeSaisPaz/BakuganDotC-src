// bdc 0x08958284 UiEquipSetPendingFlag
#include "bdc.h"

/* Sets the byte `0x08ac3410` (cleared by `UiEquipInitState`). */

void UiEquipSetPendingFlag(u8 value)

{
  g_equipPendingFlag = value;
  return;
}

