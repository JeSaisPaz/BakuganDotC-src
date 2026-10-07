// bdc 0x089ddf68 GmoMotionUpdate
#include "bdc.h"

/* Per-frame update of a GMO motion player (motion slots of 0x30 bytes, count `slotCount`,
   current `current`): with flag 0x400 in `+0x2` first updates the linked sub-players
   (`GmoMotionUpdateLinked`); then if the current slot's weight is below 1 cross-fades
   (`GmoMotionUpdateCrossfade`) and advances all weighted slots (`GmoMotionAdvanceBlended`),
   else advances the current one alone at weight 1 (`GmoMotionAdvance`). */

void GmoMotionUpdate(float dt, void *player, u32 mask)
{
  GmoMotionPlayer *p = (GmoMotionPlayer *)player;
  GmoMotionSlot *current;
  u32 count;
  u32 idx;

  if (p != NULL) {
    if ((p->flags & 0x400) != 0) {
      GmoMotionUpdateLinked(dt, player, mask);
    }
    count = p->slotCount;
    if (count != 0) {
      idx = (s32)(s16)p->current;
      if (((s32)idx < 0) || ((s32)count < (s32)idx)) {
        idx = count;
      }
      current = &p->slots[idx];
      if (current->weight < 1.0f) {
        GmoMotionUpdateCrossfade(dt, player, current);
        GmoMotionAdvanceBlended(dt, player, mask);
      } else {
        GmoMotionAdvance(dt, 1.0f, player, current, mask);
      }
    }
  }
}
