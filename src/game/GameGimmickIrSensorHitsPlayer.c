// bdc 0x088d6780 GameGimmickIrSensorHitsPlayer
#include "bdc.h"

/* Returns 1 when the player's collider (`player+0x170 → +0xf4`, virtual test `+0x14`) intersects
   the beam shape `+0x1c0` of the IR sensor gimmick (`GameGimmickIrSensorCtor`, vtables
   `0x08af303c`/`0x08af30e4`); never while save-profile word 0x2f (the guard-blind timer:
   `GameFieldGuardBlindStart` sets it to 180, `GameFieldGuardBlindEnd` clears it) is positive. */

typedef struct {
  u32 pad0;
  const VtblEntry *vtbl;
} GameGimmickIrSensorShape;

s32 GameGimmickIrSensorHitsPlayer(GameGimmickIrSensor *obj)
{
  Actor *player;
  s32 hit = 0;
  u8 scratch[20] __attribute__((aligned(4)));

  player = (Actor *)ActorFindPlayer();
  if (SaveHasProfile()) {
    if ((s32)SaveProfileGetWord(SaveGetProfile(), 0x2f) > 0) {
      player = (Actor *)0;
    }
  }
  if (player != (Actor *)0) {
    GameGimmickIrSensorShape *shape =
        (GameGimmickIrSensorShape *)((CollisionCollider *)player->bodyCollider)->shapeDesc;
    const VtblEntry *test = &shape->vtbl[2];

    if (((s32 (*)(void *, void *, void *))test->fn)((u8 *)shape + test->delta, &obj->shapeType, scratch)) {
      hit = 1;
    }
  }
  return hit;
}
