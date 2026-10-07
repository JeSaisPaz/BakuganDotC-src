// bdc 0x089dd120 GmoDlWriteTexMatrix
#include "bdc.h"

/* Builds and writes the texture matrix (`0x40` + 12 × `0x41`) for projection/environment mapping.
   The mode word comes from the global override `g_gmoTexMapOverride` or, when that is 0, from
   the material's `texMapMode` (nothing is written unless the material has flag 0x1000000). Starting
   from `g_gmoIdentityMatrix` it applies the texture and material UV transforms
   (`GmoMat4ApplyUvTransform`), an optional matrix (the material's, or
   `g_gmoTexMapOverrideMatrix` for override flag 0x4000), the model/node orientation for
   projection bits 0xf00 (normalised with `GmoMat4Normalize3` for mode 0x8000), then for mapping
   0 the model UV transform, for mapping 2 the model scale/translation
   (`GmoMat4ApplyScaleTranslate`, unskinned nodes only). Writes x, y, w of each row. */

void GmoDlWriteTexMatrix(GmoDlContext *self)
{
    ScePspFMatrix4 tex __attribute__((aligned(16)));
    ScePspFMatrix4 orient __attribute__((aligned(16)));
    union {
        float f;
        u32 u;
    } bits;
    const ScePspFMatrix4 *matrix;
    const ScePspFMatrix4 *a;
    const ScePspFMatrix4 *b;
    const float *materialUv = NULL;
    const float *textureUv = NULL;
    ScePspFVector4 *row;
    GmoAttr *material;
    u32 *p;
    u32 mode;
    u32 projection;
    u32 mapping;
    int i;

    mode = g_gmoTexMapOverride;
    if (mode != 0) {
        projection = mode & 0xf00;
        mapping = mode & 0xf;
        matrix = NULL;
        if ((mode & 0x4000) != 0) {
            matrix = &g_gmoTexMapOverrideMatrix;
        }
    } else {
        material = self->material;
        if ((material->flags & 0x1000000) == 0) {
            return;
        }
        mode = material->texMapMode;
        matrix = (const ScePspFMatrix4 *)material->block;
        materialUv = (const float *)material->data;
        textureUv = GmoTextureGetUvTransform(self->texture);
        projection = mode & 0xf00;
        mapping = mode & 0xf;
    }
    tex = g_gmoIdentityMatrix;
    if (textureUv != NULL) {
        GmoMat4ApplyUvTransform(&tex, &tex, textureUv);
    }
    if (materialUv != NULL) {
        GmoMat4ApplyUvTransform(&tex, &tex, materialUv);
    }
    if (matrix != NULL) {
        GmoMat4Mul(&tex, &tex, matrix);
    }
    if (projection != 0) {
        a = (const ScePspFMatrix4 *)self->model->rootMatrix;
        b = (const ScePspFMatrix4 *)self->node->localMatrix;
        if (projection == 0x100) {
            a = &g_gmoIdentityMatrix;
        }
        if (self->node->boneCount != 0) {
            b = &g_gmoIdentityMatrix;
        }
        GmoMat4Mul(&orient, a, b);
        if ((mode & 0x8000) != 0) {
            GmoMat4Normalize3(&orient, &orient);
        }
        GmoMat4Mul(&tex, &tex, &orient);
    }
    if (mapping == 0) {
        GmoMat4ApplyUvTransform(&tex, &tex, self->model->uvTransform);
    } else if (mapping == 2 && self->node->boneCount == 0) {
        GmoMat4ApplyScaleTranslate(&tex, &tex, (const ScePspFVector4 *)self->model->scaleVec);
    }
    p = self->cur;
    self->cur = p + 1;
    *p = 0x40000000;
    row = &tex.x;
    for (i = 0; i < 4; i++) {
        bits.f = row->x;
        p = self->cur;
        self->cur = p + 1;
        *p = (bits.u >> 8) | 0x41000000;
        bits.f = row->y;
        p = self->cur;
        self->cur = p + 1;
        *p = (bits.u >> 8) | 0x41000000;
        bits.f = row->w;
        p = self->cur;
        self->cur = p + 1;
        *p = (bits.u >> 8) | 0x41000000;
        row++;
    }
}
