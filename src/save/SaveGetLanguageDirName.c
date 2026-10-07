// bdc 0x0880dc54 SaveGetLanguageDirName
#include "bdc.h"

/* Returns the language directory name used in `data/2d/%s/...` paths: indexes a 14-entry table
   (`g_languageDirNames`) by the profile language (`SaveProfileGetLanguage`; index 1 = default when no
   profile exists). Values seen: `us`, `fre_eu`, `spe`, `ger`, `ita`, `dut`, `swe`. Used by
   `ScriptOpPackage` for the language-specific packages. */

const char *SaveGetLanguageDirName(void)

{
  s32 index = 1;
  const char *table[14];

  memcpy(table, g_languageDirNames, sizeof(table));
  if (SaveHasProfile()) {
    index = SaveProfileGetLanguage(SaveGetProfile());
  }
  return table[index];
}
