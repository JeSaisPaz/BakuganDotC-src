// bdc 0x0880db68 SaveProfileGetLanguage
#include "bdc.h"

/* Returns the game language stored in the profile holder: `**(s32 **)(profile + 8)`. Values are the
   table index used for sound-language directories: 1 `EN`, 2 `FR`, 3 `ES`, 4 `DE`, 5 `IT`, 6 `NE`
   (probably Dutch). */

s32 SaveProfileGetLanguage(SaveProfile *self)

{
  return *self->language;
}

