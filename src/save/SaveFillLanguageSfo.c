// bdc 0x0880e2f8 SaveFillLanguageSfo
#include "bdc.h"

/* Fills the PARAM.SFO strings of the language-setting save file: title `"BAKUGAN2 PORTABLE"` into
   `title`, `"Language Setting"` at `sfo + 0x80` and the current language's native name
   (`日本語`, `Español`, `Nederlands`, `Svenska`, … from `g_languageSfoStrings` `[3 + id]`,
   `"DEFAULT"` for ids ≥ 13) at `sfo + 0x100`. Called by `SysUtilSavedataHandlerRequest`. */

void SaveFillLanguageSfo(char *title, char *sfo)

{
  const char *src;
  u32 language;

  strcpy(title, g_languageSfoStrings[0]);
  strcpy(sfo + 0x80, g_languageSfoStrings[1]);
  language = SaveProfileGetLanguage(SaveGetProfile());
  src = g_languageSfoStrings[16];
  if (language < 13) {
    src = g_languageSfoStrings[3 + language];
  }
  strcpy(sfo + 0x100, src);
}
