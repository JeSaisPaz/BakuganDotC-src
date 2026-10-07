// bdc 0x08a1d530 GmoModelUpdateTextureFlags
#include "bdc.h"

/* Recomputes texture-dependent flags of a GMO model data block (0xc0-byte header, see
   `GmoModelCreate`): for every layer (0x40-byte records of each material) sets bit 0x800000 of
   `+0x20` when it has its own data (`+0x28`) or its texture (`GmoModelGetTexture` →
   `GmoRecGetNext`) has a UV-transform block (`GmoTextureGetUvTransform`), clearing it
   otherwise; sets model flag 0x400 (`+2`) when any texture is dynamic (`GmoTextureIsDynamic`,
   flag bit 4), clearing it otherwise. */

void GmoModelUpdateTextureFlags(GmoModel *self)
{
  int matIdx;
  int attrIdx;
  int texIdx;
  u16 dynamic;

  if (self == NULL) {
    return;
  }
  for (matIdx = 0; matIdx < (int)self->materialCount; matIdx++) {
    GmoMaterial *mat = &((GmoMaterial *)self->materials)[matIdx];
    for (attrIdx = 0; attrIdx < (int)mat->attrCount; attrIdx++) {
      GmoAttr *attr = &mat->attrs[attrIdx];
      void *data = attr->data;
      attr->flags &= ~0x800000u;
      if (data != NULL) {
        attr->flags |= 0x800000u;
      } else {
        void *rec = GmoModelGetTexture(self, attr->layerRef);
        GmoTexture *tex = (GmoTexture *)GmoRecGetNext(rec);
        if (GmoTextureGetUvTransform(tex) != NULL) {
          attr->flags |= 0x800000u;
        }
      }
    }
  }
  dynamic = 0;
  for (texIdx = 0; texIdx < (int)self->textureCount; texIdx++) {
    if (GmoTextureIsDynamic(((GmoLayer *)self->textures)[texIdx].texture) != 0) {
      dynamic = 0x400;
    }
  }
  self->flags02 = (self->flags02 & ~0x400) | dynamic;
}
