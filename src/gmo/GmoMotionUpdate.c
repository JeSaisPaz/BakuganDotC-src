// bdc 0x089ddf68 GmoMotionUpdate
#include "bdc.h"

/* Per-frame update of a GMO model's motions (`GmoModel`: motion slots of 0x30 bytes, count
   `motionCount`, current `motionIndex`): with flag 0x400 in `+0x2` first updates the linked sub-players
   (`GmoMotionUpdateLinked`); then if the current slot's weight is below 1 cross-fades
   (`GmoMotionUpdateCrossfade`) and advances all weighted slots (`GmoMotionAdvanceBlended`),
   else advances the current one alone at weight 1 (`GmoMotionAdvance`). */

void GmoMotionUpdate(float dt, void *player, u32 mask)
{
  GmoModel *p = (GmoModel *)player;
  GmoMotionSlot *current;
  u32 count;
  u32 idx;

  if (p != NULL) {
    if ((p->flags02 & 0x400) != 0) {
      GmoMotionUpdateLinked(dt, player, mask);
    }
    count = p->motionCount;
    if (count != 0) {
      idx = (s32)p->motionIndex;
      if (((s32)idx < 0) || ((s32)count < (s32)idx)) {
        idx = count;
      }
      current = &((GmoMotionSlot *)p->motions)[idx];
      if (current->weight < 1.0f) {
        GmoMotionUpdateCrossfade(dt, player, current);
        GmoMotionAdvanceBlended(dt, player, mask);
      } else {
        GmoMotionAdvance(dt, 1.0f, player, current, mask);
      }
    }
  }
}
