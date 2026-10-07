// bdc 0x08948394 UiBattleRecordFadeIsFinished
#include "bdc.h"

/* Returns whether the active fader has finished (`GfxFaderIsFinished`). */

bool UiBattleRecordFadeIsFinished(void)
{
  return GfxFaderIsFinished(GfxGetActiveFader());
}
