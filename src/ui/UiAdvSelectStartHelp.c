// bdc 0x08919ac4 UiAdvSelectStartHelp
#include "bdc.h"

/* Arms the help dialog of the adventure partner-select screen (`UiAdvSelectCtor`, task 376;
   cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots): clears the
   record `+0x908` and stores `kind` in its first byte (step `+0x909 = 0`). */

void UiAdvSelectStartHelp(UiAdvSelect *self, u8 kind)

{
  memset(&self->helpKind,0,4);
  self->helpKind = kind;
  return;
}

