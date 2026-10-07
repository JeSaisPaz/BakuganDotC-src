// bdc 0x0880c5c8 SaveProfileSetLanguage
#include "bdc.h"

/* Sets the game language: stores `language` in the profile's language word (`*self->language`),
   points the language string table pointer `g_langStrings` at the matching table (1 English,
   2 French, 3 Spanish, 4 German, 5 Italian, 6 Dutch, 12 Swedish; anything else falls back to the
   English table) and forwards the id to the pad state with `PadSetLanguage`; for an unknown id
   the pad gets 13 ('system default') while the profile word keeps the original value. */

void SaveProfileSetLanguage(SaveProfile *self, s32 language)
{
  *self->language = language;
  switch (language) {
  case 1:
    g_langStrings = g_langStringsEn;
    break;
  case 2:
    g_langStrings = g_langStringsFr;
    break;
  case 3:
    g_langStrings = g_langStringsEs;
    break;
  case 4:
    g_langStrings = g_langStringsDe;
    break;
  case 5:
    g_langStrings = g_langStringsIt;
    break;
  case 6:
    g_langStrings = g_langStringsNl;
    break;
  case 12:
    g_langStrings = g_langStringsSv;
    break;
  default:
    language = 13;
    g_langStrings = g_langStringsEn;
    break;
  }
  PadSetLanguage(g_padState, language);
}
