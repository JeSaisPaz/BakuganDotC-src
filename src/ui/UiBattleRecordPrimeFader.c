// bdc 0x08948300 UiBattleRecordPrimeFader
#include "bdc.h"

/* Initialises the fader slots if they are not ready (`GfxFaderIsReady`, `GfxFaderSlotsInit`)
   and stores `value` as the sort key (`+0x10`) of the active fader. */

void UiBattleRecordPrimeFader(float value)
{
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit((GfxFader *)0);
    GfxGetActiveFader()->sortKey = value;
  }
}
