// bdc 0x0889fbb8 GameGimmickCorePointUpdate
#include "bdc.h"

/* Per-frame update of the core-point gimmick (vtable `0x08af234c` slot 7): runs the distance fade
   through virtual slot 5 (`+0x2c`: near 23, far 480, position `rootMatrix[12]`, alpha `+0x178`) and
   returns when it reports "hidden" or the new alpha is `<= 0`. When the alpha changed, alpha 1
   switches the materials to opaque (`0x0889f8cc`) and any other alpha to translucent (`0x0889f8bc`)
   with `GfxModelForEachMaterial`, once per switch. Then dispatches state `+0x180` (0..2) through
   the member-pointer table `0x08a83c54`: `GameGimmickCorePointState00Idle`,
   `GameGimmickCorePointState01Collected`, `GameGimmickCorePointState02Despawn`. */

void GameGimmickCorePointUpdate(GameGimmickCorePoint *obj)
{
  const VtblEntry *e;
  float prevAlpha;
  float alpha;
  int state;

  prevAlpha = obj->base.alpha;
  e = &((const VtblEntry *)obj->base.base.base.vtable)[5];
  if (((int (*)(void *, float *, float *, float, float))e->fn)(
          (u8 *)obj + e->delta, &obj->base.base.data->rootMatrix[12], &obj->base.alpha, 23.0f,
          480.0f) != 0) {
    return;
  }
  alpha = obj->base.alpha;
  if (alpha <= 0.0f) {
    return;
  }
  if (!(alpha == prevAlpha)) {
    if (alpha == 1.0f) {
      obj->translucent = 0;
      if (obj->opaque == 0) {
        GfxModelForEachMaterial(&obj->base.base, GameGimmickCorePointMaterialSetOpaque, NULL);
        obj->opaque = 1;
      }
    } else {
      obj->opaque = 0;
      if (obj->translucent == 0) {
        GfxModelForEachMaterial(&obj->base.base, GameGimmickCorePointMaterialSetTranslucent, NULL);
        obj->translucent = 1;
      }
    }
  }
  state = obj->cpState;
  if (state >= 0 && state < 3) {
    const VtblEntry *entry = &g_gimmickCorePointStateTable[state];
    u8 *self = (u8 *)obj + entry->delta;
    void *fn = entry->fn;

    if (entry->pad != 0) {
      const VtblEntry *vt = *(const VtblEntry **)(self + (intptr_t)fn);

      vt += entry->pad;
      fn = vt->fn;
      self += vt->delta;
    }
    ((void (*)(void *))fn)(self);
  }
}
