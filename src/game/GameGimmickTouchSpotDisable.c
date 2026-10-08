// bdc 0x088db31c GameGimmickTouchSpotDisable
#include "bdc.h"

/* Vtable `0x08af3684` slot 16: when active (`+0x15e`), clears the flag, releases the looping effect
   `+0x180` from its layer (`UiSpriteLayerRelease` on `effect + 0x214`) and destroys the collider
   `+0x174`. */

void GameGimmickTouchSpotDisable(GameGimmickTouchSpot *gimmick)

{
  if ((gimmick->base).active != '\0') {
    (gimmick->base).active = '\0';
    if (gimmick->sprite != (void *)0x0) {
      UiSpriteLayerRelease(((GfxEffect *)gimmick->sprite)->mgr,gimmick->sprite);
    }
    if ((gimmick->base).attached == (void *)0x0) {
      (gimmick->base).attached = (void *)0x0;
    }
    else {
      CoreNode *node = (CoreNode *)(gimmick->base).attached;
      const VtblEntry *dtor = &((const VtblEntry *)node->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
      (gimmick->base).attached = (void *)0x0;
      (gimmick->base).attached = (void *)0x0;
    }
  }
  return;
}
