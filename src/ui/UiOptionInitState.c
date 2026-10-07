// bdc 0x0896fa64 UiOptionInitState
#include "bdc.h"

/* Class init of the battle-options screen `UiOption` (task 304), called by
   `UiOptionCtor`: clears the cursor and fade records, loads the option values from the profile
   (`UiOptionSyncProfile` with `store` = 0) and builds the enabled-row mask `+0xbb9` (rows 0..3;
   row 3 only when profile word 7, the battle type, is 0). When `SaveGetProfileFlag0` is set the
   cursor starts on row 1 and row 0 is disabled. */

void UiOptionInitState(UiOption *self)

{
  u32 i;
  
  memset(&self->cursor,0,1);
  memset(&self->fadeActive,0,0x10);
  UiOptionSyncProfile(self,false);
  self->enabledRows = '\0';
  i = 0;
  do {
    if (i == 3) {
      if (SaveProfileGetWord(SaveGetProfile(),7) != 0) break;
    }
    self->enabledRows = self->enabledRows | (u8)(1 << i);
    i = i + 1;
  } while (i < 4);
  if (SaveGetProfileFlag0() != 0) {
    self->cursor = '\x01';
    self->enabledRows = self->enabledRows & 0xfe;
  }
  return;
}

