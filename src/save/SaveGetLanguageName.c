// bdc 0x0880dd14 SaveGetLanguageName
#include "bdc.h"

/* Returns the language's long name from the 14-entry table `0x08aaca64` (`"English_US"`,
   `"French"`, `"Spanish"`, `"German"`, `"Italian"`, `"Dutch"`, `"Svenska"`, …), indexed by
   `SaveProfileGetLanguage` (index 1 without a profile). Used to build localized asset paths by
   `GameFieldPhaseMain` and event code. */

const char * SaveGetLanguageName(void)

{
  s32 index = 1;
  const char *table[14];

  memcpy(table, g_languageNames, sizeof(table));
  if (SaveHasProfile()) {
    index = SaveProfileGetLanguage(SaveGetProfile());
  }
  return table[index];
}
