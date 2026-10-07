// bdc 0x089abf60 UiPauseSettingsAdviceAvailable
#include "bdc.h"

/* Returns 1 when the advisor-comment toggle applies: not opened from the main menu, and either a
   field (task 500) runs or a battle (task 100) runs with script variable 8 == 1; else 0 (the
   row is greyed out). */

int UiPauseSettingsAdviceAvailable(UiPauseSettings *self)
{
  if (g_lastScreenTaskId != 300 &&
      (CoreTaskExists(500) != 0 || (CoreTaskExists(100) != 0 && g_scriptGlobalVars[8] == 1))) {
    return 1;
  }
  return 0;
}
