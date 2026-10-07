// bdc 0x0885ddc0 GfxModelApplyTextureVariant
#include "bdc.h"

/* Re-textures a model with a numbered variant: for each material `i < materialCount` it copies the
   name of the material's texture (`GfxModelGetMaterialTexture`); when the name ends in a digit
   (`'0'..'9'`) the last 4 characters (a `".NNN"` variant suffix) are cut off; for `variant > 0`
   `".%03d"` of `variant` is appended (format at `0x08a67764`). The name is looked up with
   `GfxTryFindTexture` and installed with `GfxModelSetMaterialTextureByIndex`. Stops at the
   first material whose variant texture does not exist. Returns 1 when the last lookup succeeded
   (all materials re-textured), 0 when one failed or the model has no materials. */

int GfxModelApplyTextureVariant(GfxModel *self, int variant)
{
  char name[64];
  GfxTexture *tex;
  size_t len;
  int digit;
  int i;

  i = 0;
  tex = NULL;
  if (i < self->materialCount) {
    do {
      tex = GfxModelGetMaterialTexture(self, i);
      strcpy(name, tex->name);
      len = strlen(name);
      digit = name[len - 1] - '0';
      if (digit < 0) {
        digit = -1;
      }
      if (digit >= 10) {
        digit = -1;
      }
      if (digit != -1) {
        name[len - 4] = '\0';
      }
      if (variant > 0) {
        sprintf(name, "%s.%03d", name, variant);
      }
      tex = GfxTryFindTexture(name);
      if (tex == NULL) {
        break;
      }
      GfxModelSetMaterialTextureByIndex(self, i, tex);
      i++;
    } while (i < self->materialCount);
  }
  return tex != NULL;
}
