// bdc 0x088d9da8 GameGimmickBarrierDisableCollider
#include "bdc.h"

/* Disables the collision of the barrier gimmick: sets flag `0x4` in the flag word `+0x130` of its
   collider (`gimmick+0x174`), which makes `CollisionRaycast` (mask `0x6`) and soft
   `CollisionHitQuery` queries skip it. Called when the barrier is switched off at once
   (`GameGimmickBarrierDisable` with `immediate == 1`) or has finished fading out
   (`GameGimmickBarrierState01FadeOut`). */

void GameGimmickBarrierDisableCollider(GameGimmickBarrier *gimmick)

{
  CollisionCollider *collider = (CollisionCollider *)gimmick->base.attached;

  collider->flags |= 4;
}
