// bdc 0x0881a540 CollisionColliderTickHit
#include "bdc.h"

/* Per-frame hit timer of a collider: when a new hit is pending (flag `0x10` in `+0x130`) it
   converts it into an active hit (flag bit 0), sets the active timer `+0x148 = duration(+0x174) +
   2` and, unless the hit kind `+0x16c` is 0x16 or 0x19, the cooldown `+0x140 = duration`; otherwise
   it counts `+0x140` down and counts `+0x148` down, clearing flag bit 0 when it reaches 0. Returns
   true when a new hit was taken this call (flag `0x10` was set; `v0` of the `sltu`). */

bool CollisionColliderTickHit(CoreNode *collider_)

{
  CollisionCollider *collider = (CollisionCollider *)collider_;
  u32 flags = collider->flags;
  bool newHit = (flags & 0x10) != 0;

  if (newHit) {
    s32 duration = collider->hitDuration;

    collider->flags = (flags & ~0x10u) | 1;
    collider->hitTimer = duration + 2;
    if (collider->hitKind != 0x16 && collider->hitKind != 0x19) {
      collider->cooldown = duration;
    }
  }
  else {
    s32 timer = collider->hitTimer;

    if (collider->cooldown != 0) {
      collider->cooldown--;
    }
    if (timer != 0) {
      collider->hitTimer = timer - 1;
      if (timer - 1 == 0) {
        collider->flags = flags & ~1u;
      }
    }
  }
  return newHit;
}
