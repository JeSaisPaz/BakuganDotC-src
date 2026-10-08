// bdc 0x089de054 GmoMotionUpdateCrossfade
#include "bdc.h"

/* Counts the cross-fade timer `player+0x24` down by `|dt|` and moves every slot's weight
   (`slot+0x20`) toward 1 for `current` and 0 for the others in proportion to the remaining time.
   Negative timer = no fade. */

void GmoMotionUpdateCrossfade(float dt, void *player, void *current)

{
  GmoModel *p = (GmoModel *)player;
  GmoMotionSlot *slot;
  float total;
  float left;
  float ratio;
  float target;
  s32 i;

  total = p->motionBlend;
  if (!(total < 0.0f)) {
    if (dt < 0.0f) {
      dt = -dt;
    }
    left = total - dt;
    if (left <= 0.0f) {
      left = 0.0f;
    }
    p->motionBlend = left;
    if (total == 0.0f) {
      ratio = 0.0f;
    } else {
      ratio = left / total;
    }
    for (i = 0; i < (s32)p->motionCount; i++) {
      slot = &((GmoMotionSlot *)p->motions)[i];
      if (slot == (GmoMotionSlot *)current) {
        target = 1.0f;
      } else {
        target = 0.0f;
      }
      slot->weight = (slot->weight - target) * ratio + target;
    }
  }
}
