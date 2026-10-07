// bdc 0x0880db74 SaveLanguageIsFrench
#include "bdc.h"

/* True when a profile exists and its language (`SaveProfileGetLanguage`) is 2 (French). Used by
   the text layout code (`UiTextMeasure`, `UiTextPrinterPrint`), e.g. for French spacing before
   punctuation. */

bool SaveLanguageIsFrench(void)
{
  s32 language = 0;

  if (SaveHasProfile()) {
    language = SaveProfileGetLanguage(SaveGetProfile());
  }
  return language == 2;
}
