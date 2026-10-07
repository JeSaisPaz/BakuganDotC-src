// bdc 0x089f7820 GfxTryFindTexture
#include "bdc.h"

/* Looks up a loaded texture by name: walks `g_textureList` comparing `name` case-insensitively
   (`strcasecmp`) with each texture's name (`+0x18`) and returns the first match, or NULL. Unlike
   `GfxFindTexture` and `GfxFindTextureOrNull` it has no `"FeedBackTex"`/`"NonTexture"`
   fallback, so callers can test whether a texture exists. */

void *GfxTryFindTexture(const char *name)
{
  GfxTexture *tex;

  for (tex = (GfxTexture *)g_textureList; tex != NULL; tex = tex->next) {
    if (strcasecmp(name, tex->name) == 0) {
      return tex;
    }
  }
  return NULL;
}
