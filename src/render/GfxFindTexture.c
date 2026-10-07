// bdc 0x089f76f0 GfxFindTexture
#include "bdc.h"

/* Looks up a loaded texture by name: walks `g_textureList` comparing `name` case-insensitively
   against each texture's name (`+0x18`) and returns the first match. If none matches it returns the
   static `g_feedbackTexture` when `name` is exactly `"FeedBackTex"`, otherwise the fallback
   `"NonTexture"` texture from `GfxGetNullTexture` — it never returns NULL. */

void *GfxFindTexture(const char *name)

{
  GfxTexture *tex;

  for (tex = (GfxTexture *)g_textureList; tex != NULL; tex = tex->next) {
    if (strcasecmp(name, tex->name) == 0) {
      return tex;
    }
  }
  if (strcmp(name, "FeedBackTex") == 0) {
    return &g_feedbackTexture;
  }
  strcmp(name, "NonTexture");
  return GfxGetNullTexture();
}
