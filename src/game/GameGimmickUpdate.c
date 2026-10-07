// bdc 0x088d931c GameGimmickUpdate
#include "bdc.h"

/* Per-frame update of the gimmick base (vtable `0x08af3314` slot 7): runs the distance fade through
   virtual slot 5 (`+0x2c`: near 23, far 480, camera-relative, alpha `+0x6c`), sets the visible flag
   `+0x164` when it reports "hidden", then the model update `GfxModelUpdateAndApplyMotion`. */

void GameGimmickUpdate(GameGimmick *gimmick)

{
  const VtblEntry *e;
  int r;

  e = &((const VtblEntry *)gimmick->base.base.vtable)[5];
  gimmick->hidden = 0;
  r = ((int (*)(void *, float *, float *, float, float))e->fn)(
      (u8 *)gimmick + e->delta, &gimmick->base.data->rootMatrix[12], &gimmick->base.ambient[3],
      23.0f, 480.0f);
  if (r != 0) {
    gimmick->hidden = 1;
  }
  GfxModelUpdateAndApplyMotion(&gimmick->base);
}
