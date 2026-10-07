// bdc 0x088247e8 GfxEffectSetColorAttached
#include "bdc.h"

/* Copies the RGBA colour `rgba` (4 floats, 16-byte aligned) to the colour of every effect of the
   manager `mgr` (sprite list head) whose definition id equals `id` (any when -1) and whose attach
   pointer equals `attach` (any when NULL). */

void GfxEffectSetColorAttached(GfxEffectMgr *mgr, s32 id, float *attach, const float *rgba)
{
  GfxEffect *fx;
  GfxEffect *nextFx;

  for (fx = (GfxEffect *)mgr->base.head; fx != NULL; fx = nextFx) {
    nextFx = (GfxEffect *)fx->base.next;
    if ((id == -1 || fx->id == id) && (attach == NULL || fx->attachPos == attach)) {
      fx->color[0] = rgba[0];
      fx->color[1] = rgba[1];
      fx->color[2] = rgba[2];
      fx->color[3] = rgba[3];
    }
  }
}
