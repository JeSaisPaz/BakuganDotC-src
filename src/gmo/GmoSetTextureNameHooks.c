// bdc 0x08a13b10 GmoSetTextureNameHooks
#include "bdc.h"

/* Installs the GMO library's external-texture hooks by name: `lookup` (`0x08afceb4`, called through
   `GmoHookFindTextureByName`) and `release` (`0x08afceac`, called through
   `GmoHookReleaseTexture`). */

void GmoSetTextureNameHooks(void *lookup, void *release)

{
  g_gmoTextureNameLookup = lookup;
  g_gmoTextureNameRelease = release;
  return;
}

