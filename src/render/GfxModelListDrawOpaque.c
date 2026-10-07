// bdc 0x089e0cc4 GfxModelListDrawOpaque
#include "bdc.h"

/* Draws the opaque models of a GfxModel chain (`first`, linked by `base.next`) into `*list`; does
   nothing when `first` is NULL or drawing is globally disabled (g_gmoDrawDisabled). Writes the
   common GE state (`0xc6000707`, `0xc7000000`, `0xd0000000 | bits(0.8f) >> 8`, `0xc8880002`),
   then calls each model's draw method (vtable `+0x40/+0x44`) for those whose alpha
   (`ambient[3]`) is not below 1. With `sorted` the models are first collected with their squared
   distance from the active camera's eye to their root-matrix translation into a 256-pair stack
   array (no bound check) and sorted back-to-front (`GfxCombSortByDepth`); otherwise, while
   g_gmoDepthWriteOverride is set, the chain is drawn twice: with g_gfxModelDrawPass = 2
   (translucent meshes) then 1 (opaque meshes), resetting it to 0 afterwards. Translucent models are
   drawn by `GfxModelListDrawTranslucent`. */

typedef struct GfxModelDepthPair {
  GfxModel *model;
  float depth;
} GfxModelDepthPair;

static void GfxModelListDrawOne(GfxModel *model, u32 **list)
{
  const GfxModelVtable *vt = (const GfxModelVtable *)model->base.vtable;
  vt->draw((u8 *)model + vt->drawAdjust, list);
}

void GfxModelListDrawOpaque(u32 **list, void *first, bool sorted)
{
  GfxModelDepthPair pairs[256];
  float eye[4] __attribute__((aligned(16)));
  union {
    float f;
    u32 u;
  } alphaRef;
  GfxModel *model = (GfxModel *)first;
  u32 *p;
  s32 count = 0;
  s32 i;

  if (model == NULL || g_gmoDrawDisabled) {
    return;
  }
  eye[0] = g_gfxActiveCamera->eye[0];
  eye[1] = g_gfxActiveCamera->eye[1];
  eye[2] = g_gfxActiveCamera->eye[2];
  eye[3] = g_gfxActiveCamera->eye[3];
  (*list)[0] = 0xc6000707;
  (*list)[1] = 0xc7000000;
  alphaRef.f = 0.8f;
  (*list)[2] = (alphaRef.u >> 8) | 0xd0000000;
  p = *list;
  p[3] = 0xc8880002;
  *list = p + 4;

  if (sorted) {
    do {
      if (!(model->ambient[3] < 1.0f)) {
        /* vsub.q + vdot.t: squared distance eye -> root translation */
        const float *t = &model->data->rootMatrix[12];
        float dx = eye[0] - t[0];
        float dy = eye[1] - t[1];
        float dz = eye[2] - t[2];
        pairs[count].depth = dx * dx + dy * dy + dz * dz;
        pairs[count].model = model;
        count++;
      }
      model = (GfxModel *)model->base.next;
    } while (model != NULL);
    if (count == 0) {
      return;
    }
    GfxCombSortByDepth(pairs, count);
    for (i = 0; i < count; i++) {
      GfxModelListDrawOne(pairs[i].model, list);
    }
  } else if (g_gmoDepthWriteOverride) {
    GfxModel *m = model;
    g_gfxModelDrawPass = 2;
    do {
      if (!(m->ambient[3] < 1.0f)) {
        GfxModelListDrawOne(m, list);
      }
      m = (GfxModel *)m->base.next;
    } while (m != NULL);
    g_gfxModelDrawPass = 1;
    do {
      if (!(model->ambient[3] < 1.0f)) {
        GfxModelListDrawOne(model, list);
      }
      model = (GfxModel *)model->base.next;
    } while (model != NULL);
    g_gfxModelDrawPass = 0;
  } else {
    do {
      if (!(model->ambient[3] < 1.0f)) {
        GfxModelListDrawOne(model, list);
      }
      model = (GfxModel *)model->base.next;
    } while (model != NULL);
  }
}
