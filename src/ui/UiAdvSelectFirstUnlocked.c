// bdc 0x08917674 UiAdvSelectFirstUnlocked
#include "bdc.h"

/* Returns the index of the first of the six candidate slots of the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner,
   locked, ?}` slots) whose locked byte (`+0x8a2 + 4*i`) is clear, or 0. */

int UiAdvSelectFirstUnlocked(UiAdvSelect *self)

{
  int i;
  
  for (i = 0; i < 6; i++) {
    if (self->candidates[i].locked == '\0') {
      return i;
    }
  }
  return 0;
}

