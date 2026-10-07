// bdc 0x08a13b90 GmoHookReleaseTexture
#include "bdc.h"

/* Calls the external texture release hook (`0x08afceac`, `GmoSetTextureNameHooks`) with `name`
   (the empty string `0x08aa5214` when NULL); nothing when no hook is installed. */

void GmoHookReleaseTexture(const char *name)

{
  if (g_gmoTextureNameRelease == NULL) {
    return;
  }
  if (name != NULL) {
    ((void (*)(const char *))g_gmoTextureNameRelease)(name);
    return;
  }
  ((void (*)(const char *))g_gmoTextureNameRelease)(g_gmoEmptyString);
  return;
}
