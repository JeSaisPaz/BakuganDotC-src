// bdc 0x089dcfec GmoDlWriteUvTransform
#include "bdc.h"

/* Writes the texture UV offset (`0x4a`/`0x4b`) and scale (`0x48`/`0x49`): the model default
   `model->uvTransform`, combined, when the material has flag 0x800000, with the material's
   (`data`) and the texture's (`GmoTextureGetUvTransform`) UV transforms (`GmoUvTransformCombine`). */

void GmoDlWriteUvTransform(GmoDlContext *self)
{
  GmoAttr *mat = self->material;
  float *uv = self->model->uvTransform;
  float combined[4] __attribute__((aligned(16)));
  union { float f; u32 u; } bits[4];
  u32 *p;

  if ((mat->flags & 0x800000) != 0) {
    float *matUv = mat->data;
    float *texUv = GmoTextureGetUvTransform(self->texture);

    if (matUv != NULL) {
      uv = GmoUvTransformCombine(combined, matUv, uv);
    }
    if (texUv != NULL) {
      uv = GmoUvTransformCombine(combined, texUv, uv);
    }
  }

  bits[0].f = uv[0];
  bits[1].f = uv[1];
  p = self->cur;
  self->cur = p + 1;
  *p = bits[0].u >> 8 | 0x4a000000;
  p = self->cur;
  self->cur = p + 1;
  *p = bits[1].u >> 8 | 0x4b000000;

  bits[2].f = uv[2];
  bits[3].f = uv[3];
  p = self->cur;
  self->cur = p + 1;
  *p = bits[2].u >> 8 | 0x48000000;
  p = self->cur;
  self->cur = p + 1;
  *p = bits[3].u >> 8 | 0x49000000;
}
