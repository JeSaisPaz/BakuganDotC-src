// bdc 0x089de1b0 GmoMotionAdvanceBlended
#include "bdc.h"

/* Advances every motion slot with a positive weight (`GmoMotionAdvance`), passing `weight /
   runningSum` so successive blends accumulate to the weighted average. */

void GmoMotionAdvanceBlended(float dt, void *player, u32 mask)

{
  GmoModel *p = (GmoModel *)player;
  GmoMotionSlot *slot;
  float sum;
  float weight;
  int i;

  sum = 0.0f;
  slot = (GmoMotionSlot *)p->motions;
  for (i = 0; i < (int)p->motionCount; i++, slot++) {
    weight = slot->weight;
    if (!(weight <= 0.0f)) {
      sum = sum + weight;
      GmoMotionAdvance(dt,weight / sum,player,slot,mask);
    }
  }
}
