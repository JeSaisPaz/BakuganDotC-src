// bdc 0x08993458 UiUnlockCodeNormalizeCode
#include "bdc.h"

/* Copies the entered characters `+0xb4[i]` of `UiUnlockCode` into the comparison
   buffer `+0xdc[i]`, mapping empty slots and blank characters to 0x27 (the space key). */

void UiUnlockCodeNormalizeCode(UiUnlockCode *self)
{
  int i;

  for (i = 0; i < self->digitCount; i++) {
    int c = self->entered[i];
    if (c == -1 || c == 0x27) {
      self->normalized[i] = c;
    } else {
      s32 out = 0x27;
      if (strcmp(g_unlockKeyTable[c], g_unlockSpaceString) != 0) {
        out = self->entered[i];
      }
      self->normalized[i] = out;
    }
  }
}
