// bdc 0x089affc8 UiBattleModeSelectInitEntryEnabled
#include "bdc.h"

/* Marks both entries of `UiBattleModeSelect` as selectable: copies the
   constant pair `{1, 1}` into the per-entry enable bytes `+0x579[0..1]`. */

void UiBattleModeSelectInitEntryEnabled(UiBattleModeSelect *self)
{
  s8 init[2];
  s32 i;

  init[0] = 1;
  init[1] = 1;
  i = 0;
  do {
    self->entryEnabled[i] = init[i];
    i++;
  } while (i < 2);
}
