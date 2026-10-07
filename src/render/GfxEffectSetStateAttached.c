// bdc 0x08824750 GfxEffectSetStateAttached
#include "bdc.h"

/* Writes `state` to `effect+0x170` of every effect on the manager `mgr` whose definition id
   (`+0x16c`) equals `id` (or any id when `id == -1`) and whose attach pointer (`+0x160`) equals
   `attach` (any when `attach == NULL`). */

void GfxEffectSetStateAttached(GfxEffectMgr *mgr, int id, void *attach, int state)

{
  GfxEffect *effect = (GfxEffect *)mgr->base.head;
  GfxEffect *next;

  while (effect != NULL) {
    next = (GfxEffect *)effect->base.next;
    if ((id == -1) || (effect->id == id)) {
      if (attach == NULL) {
        effect->key = state;
      } else if ((void *)effect->attachPos == attach) {
        effect->key = state;
      }
    }
    effect = next;
  }
}
