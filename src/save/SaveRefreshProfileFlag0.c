// bdc 0x0880d2c8 SaveRefreshProfileFlag0
#include "bdc.h"

/* Recomputes `g_profileFlag0`: 1 when a profile exists and bit 0 of its flag word (word 0) is
   set, else 0. Read back with `SaveGetProfileFlag0`; called from screen constructors such as
   `UiPauseCtor`. */

void SaveRefreshProfileFlag0(void)
{
  g_profileFlag0 = 0;
  if (SaveHasProfile()) {
    SaveProfile *self = SaveGetProfile();
    u32 word = SaveProfileGetWord(self, 0);
    if ((word & 1) != 0) {
      g_profileFlag0 = 1;
    }
  }
}
