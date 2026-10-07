// bdc 0x08a13b38 GmoHookFindTextureByName
#include "bdc.h"

/* Calls the external texture lookup hook (`0x08afceb4`, `GmoSetTextureNameHooks`) with `name`
   (the empty string `0x08aa5214` when NULL) and returns the texture it finds; NULL when no hook is
   installed. */

void *GmoHookFindTextureByName(const char *name)

{
  if (g_gmoTextureNameLookup == NULL) {
    return NULL;
  }
  if (name != NULL) {
    return ((void *(*)(const char *))g_gmoTextureNameLookup)(name);
  }
  return ((void *(*)(const char *))g_gmoTextureNameLookup)(g_gmoEmptyString);
}
