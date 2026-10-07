// bdc 0x089ab760 UiPauseSettingsHasExtraEntry
#include "bdc.h"

/* Returns 1 when the settings screen was opened from the main menu (previous screen id `g_lastScreenTaskId`
   == 300) or outside a field (task 500) / battle (task 100), i.e. when the extra entry 6 is
   offered; 0 otherwise. */

int UiPauseSettingsHasExtraEntry(UiPauseSettings *self)

{
  if ((g_lastScreenTaskId != 300) &&
     ((CoreTaskExists(500) != 0 || CoreTaskExists(100) != 0))) {
    return 0;
  }
  return 1;
}

