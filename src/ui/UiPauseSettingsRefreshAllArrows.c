// bdc 0x089ad8fc UiPauseSettingsRefreshAllArrows
#include "bdc.h"

/* Runs `UiPauseSettingsRefreshRowArrows` for rows 0..3. */

void UiPauseSettingsRefreshAllArrows(UiPauseSettings *self)
{
  int i = 0;

  do {
    UiPauseSettingsRefreshRowArrows(self, (u8)i);
    i++;
  } while (i < 4);
}
