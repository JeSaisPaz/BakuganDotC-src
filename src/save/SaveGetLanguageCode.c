// bdc 0x0880dcb4 SaveGetLanguageCode
#include "bdc.h"

/* Returns the two-letter region code of the current language from the 14-entry table `0x08aaca2c`:
   `"fr"`, `"es"`, `"de"`, `"it"`, `"nl"`, `"se"` for languages 2–6 and 12, `"eu"` (English)
   otherwise; `"eu"` without a profile (index 1). */

const char * SaveGetLanguageCode(void)

{
  s32 index = 1;
  const char *table[14];

  memcpy(table, g_languageCodes, sizeof(table));
  if (SaveHasProfile()) {
    index = SaveProfileGetLanguage(SaveGetProfile());
  }
  return table[index];
}
