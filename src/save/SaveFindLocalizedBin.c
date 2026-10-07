// bdc 0x0880dbc0 SaveFindLocalizedBin
#include "bdc.h"

/* Looks up the localized resource `<base>_<lang>.bin` in the package chain `g_ioLzsPackages`
   (`CorePackChainFind`), where `<lang>` is the two-letter region code from
   `SaveGetLanguageCode` (`"eu"` without a profile). Returns the `CorePackChainFind` result. */

void * SaveFindLocalizedBin(const char *base)
{
  char name[64];

  strcpy(name, base);
  strcat(name, "_");
  if (SaveHasProfile()) {
    SaveGetProfile();
    strcat(name, SaveGetLanguageCode());
  }
  else {
    strcat(name, "eu");
  }
  strcat(name, ".bin");
  return CorePackChainFind(g_ioLzsPackages, name);
}
