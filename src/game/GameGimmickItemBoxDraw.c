// bdc 0x088d74b4 GameGimmickItemBoxDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the item box (breakable obstacle) gimmick (`GameGimmickItemBoxCtor`,
   vtables `0x08af30f4`/`0x08af319c`): `GameGimmickDraw`, then fades the break alpha `breakAlpha`
   by 0.03 per call while it is below 1 (snapping to 0 below 0.25). While it is above 0, draws the
   shared break-effect model `g_itemBoxBreakEffect1` at the box's root matrix with its Y row scaled
   by the alpha and the box's ambient alpha; at alpha exactly 1 also draws
   `g_itemBoxBreakEffect2` at the box's matrix (the ambient alpha is copied into effect 1, not 2).
   Always clears `g_itemBoxEffectUpdated`. */

void GameGimmickItemBoxDraw(GameGimmickItemBox *obj, u32 **dl)

{
  float *dst;
  const float *src;
  const VtblEntry *e;
  float alpha;
  float scale;
  int i;

  GameGimmickDraw(&obj->base, dl);
  if (obj->breakAlpha < 1.0f) {
    alpha = obj->breakAlpha - 0.03f;
    obj->breakAlpha = alpha;
    if (alpha < 0.25f) {
      obj->breakAlpha = 0.0f;
    }
  }
  if (!(obj->breakAlpha <= 0.0f)) {
    if (g_itemBoxBreakEffect1 != NULL) {
      dst = g_itemBoxBreakEffect1->data->rootMatrix;
      src = obj->base.base.data->rootMatrix;
      for (i = 0; i < 16; i++) {
        dst[i] = src[i];
      }
      /* scale rows 0..2 (all four lanes) by (1, alpha, 1) */
      dst = g_itemBoxBreakEffect1->data->rootMatrix;
      scale = obj->breakAlpha;
      for (i = 0; i < 4; i++) {
        dst[i] = dst[i] * 1.0f;
        dst[4 + i] = dst[4 + i] * scale;
        dst[8 + i] = dst[8 + i] * 1.0f;
      }
      /* translation row again */
      dst = g_itemBoxBreakEffect1->data->rootMatrix;
      src = obj->base.base.data->rootMatrix;
      for (i = 12; i < 16; i++) {
        dst[i] = src[i];
      }
      g_itemBoxBreakEffect1->ambient[3] = obj->base.base.ambient[3];
      e = &((const VtblEntry *)g_itemBoxBreakEffect1->base.vtable)[8];
      ((void (*)(void *, u32 **))e->fn)((u8 *)g_itemBoxBreakEffect1 + e->delta, dl);
    }
    if (obj->breakAlpha == 1.0f && g_itemBoxBreakEffect2 != NULL) {
      g_itemBoxBreakEffect1->ambient[3] = obj->base.base.ambient[3];
      dst = g_itemBoxBreakEffect2->data->rootMatrix;
      src = obj->base.base.data->rootMatrix;
      for (i = 0; i < 16; i++) {
        dst[i] = src[i];
      }
      e = &((const VtblEntry *)g_itemBoxBreakEffect2->base.vtable)[8];
      ((void (*)(void *, u32 **))e->fn)((u8 *)g_itemBoxBreakEffect2 + e->delta, dl);
    }
  }
  g_itemBoxEffectUpdated = 0;
  return;
}
