// bdc 0x088a4ecc ActorStageObjEggCrystalHatch
#include "bdc.h"

/* Hatches a unit from the egg crystal. Returns 0 without doing anything when 4 or more units
   are active (`BtlCountUnitsWithVirtual54Or64`). Otherwise looks the owner up in the battle
   list (`BtlBakuganListFind`, NULL when gone), clears the hit timer and sets flags 1|4|0x40 on
   both colliders of the companion `unit` (when it exists), creates the stored Bakugan kind
   `hatchKind` in mode 2 150 units above the ground under the crystal (ground searched from 1500
   units above it, heading `rot[1]`) (`BtlCreateBakugan`), collider layer 5, `allyIndex` =
   `ownerSlot`, `ally` = owner, ball entry (`BtlBakuganBeginBallEntry`), HP gauge, player slot
   4, `flag6b0` = `unitFlag`, AI level (`BtlBakuganSetAiLevel`), spawn params `targetPoint`
   (`BtlUnitApplySpawnParams`), `flag6bc` = 1, and runs its vtable entry 7 (`+0x38`/`+0x3c`).
   Returns 1. */

int ActorStageObjEggCrystalHatch(ActorStageObjEggCrystal *self)

{
  float ground[4] __attribute__((aligned(16)));
  float spawn[4] __attribute__((aligned(16)));
  void *ally;
  CollisionCollider *collider;
  BtlCpuUnit *hatched;
  const VtblEntry *entry;
  int i;

  if (BtlCountUnitsWithVirtual54Or64() >= 4) {
    return 0;
  }
  ally = BtlBakuganListFind((BtlBakugan *)self->owner);
  if (self->unit != NULL) {
    collider = ((BtlBakugan *)self->unit)->collider0;
    collider->hitTimer = 0;
    collider->flags = collider->flags | 1;
    collider = ((BtlBakugan *)self->unit)->collider0;
    collider->flags = collider->flags | 4;
    collider = ((BtlBakugan *)self->unit)->collider0;
    collider->flags = collider->flags | 0x40;
    collider = ((BtlBakugan *)self->unit)->collider1;
    collider->hitTimer = 0;
    collider->flags = collider->flags | 1;
    collider = ((BtlBakugan *)self->unit)->collider1;
    collider->flags = collider->flags | 4;
    collider = ((BtlBakugan *)self->unit)->collider1;
    collider->flags = collider->flags | 0x40;
  }
  for (i = 0; i < 4; i++) {
    spawn[i] = self->base.base.pos[i];
  }
  spawn[1] = spawn[1] + 1500.0f;
  CollisionFindGroundPoint(ground, spawn, 0x3fbf2500);
  for (i = 0; i < 4; i++) {
    spawn[i] = ground[i];
  }
  spawn[1] = spawn[1] + 150.0f;
  spawn[3] = self->base.base.rot[1];
  hatched = (BtlCpuUnit *)BtlCreateBakugan(self->hatchKind, 2, spawn);
  BtlBakuganSetColliderLayer(&hatched->base, 5);
  hatched->allyIndex = self->ownerSlot;
  hatched->ally = (BtlBakugan *)ally;
  BtlBakuganBeginBallEntry(&hatched->base, 1);
  BtlBakuganCreateHpGauge(&hatched->base);
  hatched->base.playerSlot = 4;
  hatched->flag6b0 = self->unitFlag;
  BtlBakuganSetAiLevel(&hatched->base, self->aiLevel);
  for (i = 0; i < 4; i++) {
    ground[i] = self->targetPoint[i];
  }
  BtlUnitApplySpawnParams(&hatched->base, ground);
  hatched->flag6bc = 1;
  entry = &((const VtblEntry *)hatched->base.base.base.vtable)[7];
  ((void (*)(void *))entry->fn)((u8 *)hatched + entry->delta);
  return 1;
}
