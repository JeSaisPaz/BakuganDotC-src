// bdc 0x08a13b24 GmoSetTextureIdHooks
#include "bdc.h"

/* Installs the GMO library's texture-by-id hook (`0x08afceb0`, called through
   `GmoHookFindTextureById`) and a second hook word `0x08afcea8`. */

void GmoSetTextureIdHooks(void *lookupById, void *other)

{
  g_gmoTextureIdLookup = lookupById;
  g_gmoTextureIdOther = other;
  return;
}

