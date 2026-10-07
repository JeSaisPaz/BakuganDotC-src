// bdc 0x089de054 GmoMotionUpdateCrossfade
#include "bdc.h"

/* Counts the cross-fade timer `player+0x24` down by `|dt|` and moves every slot's weight
   (`slot+0x20`) toward 1 for `current` and 0 for the others in proportion to the remaining time.
   Negative timer = no fade. */

void GmoMotionUpdateCrossfade(float dt, void *player, void *current)

{
  GmoMotionPlayer *p = (GmoMotionPlayer *)player;
  GmoMotionSlot *slot;
  float total;
  float left;
  float ratio;
  float target;
  s32 i;

  total = p->fadeTime;
  if (!(total < 0.0f)) {
    if (dt < 0.0f) {
      dt = -dt;
    }
    left = total - dt;
    if (left <= 0.0f) {
      left = 0.0f;
    }
    p->fadeTime = left;
    if (total == 0.0f) {
      ratio = 0.0f;
    } else {
      ratio = left / total;
    }
    for (i = 0; i < (s32)p->slotCount; i++) {
      slot = &p->slots[i];
      if (slot == (GmoMotionSlot *)current) {
        target = 1.0f;
      } else {
        target = 0.0f;
      }
      slot->weight = (slot->weight - target) * ratio + target;
    }
  }
}
