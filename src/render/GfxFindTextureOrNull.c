// bdc 0x089f77ac GfxFindTextureOrNull
#include "bdc.h"

/* Looks up a texture by name in `g_textureList` (case-insensitive, `strcasecmp`); returns the
   `"NonTexture"` fallback (`GfxGetNullTexture`) when not found. `GfxFindTexture` without the
   `"FeedBackTex"` special case. */

void *GfxFindTextureOrNull(char *name)

{
  GfxTexture *tex;

  for (tex = (GfxTexture *)g_textureList; tex != NULL; tex = tex->next) {
    if (strcasecmp(name, tex->name) == 0) {
      return tex;
    }
  }
  return GfxGetNullTexture();
}
