// bdc 0x088a6070 ActorStageObjMineDetect
#include "bdc.h"

/* Proximity test of a mine. Returns 0 while a battle demo runs (`BtlIsDemoRunning`) or outside a
   battle (`BtlGetBakuganList` NULL). Otherwise returns 1 and stores the intruder in
   `triggeredBy` for the first unit of the Bakugan list that answers virtual slot 10 or 12 and whose
   anchor position (`anchorMatrix[3]`) lies within squared distance
   `triggerRadius² + combat.stats->reachRadius²` of the mine, else for the first attack of
   `g_btlAttackList` whose position (linked-target attacks, `BtlAttackUsesLinkedTarget`: the
   position of `effect2`, skipped when that is NULL) lies within `triggerRadius² + radius²`
   (`BtlAttackGetRadius`); the attack's `owner` is stored then. Returns 0 when nothing is that
   close. */

static float MineDistSq(const float *other, const float *self)
{
  float dx = other[0] - self[0];
  float dy = other[1] - self[1];
  float dz = other[2] - self[2];

  return dx * dx + dy * dy + dz * dz;
}

int ActorStageObjMineDetect(ActorStageObjMine *self)
{
  BtlBakugan **list;
  BtlBakugan *unit;
  BtlAttack *attack;
  const VtblEntry *vtbl;
  float triggerSq;
  float reach;
  float attackSq;
  float distSq;

  list = BtlGetBakuganList();
  if (BtlIsDemoRunning() || list == NULL) {
    return 0;
  }
  unit = *list;
  triggerSq = self->base.triggerRadius * self->base.triggerRadius;
  for (; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
    vtbl = (const VtblEntry *)unit->base.base.vtable;
    if (((int (*)(void *))vtbl[10].fn)((u8 *)unit + vtbl[10].delta) == 0) {
      vtbl = (const VtblEntry *)unit->base.base.vtable;
      if (((int (*)(void *))vtbl[12].fn)((u8 *)unit + vtbl[12].delta) == 0) {
        continue;
      }
    }
    reach = unit->combat.stats->reachRadius;
    distSq = MineDistSq(unit->anchorMatrix[3], self->base.base.pos);
    if (distSq <= triggerSq + reach * reach) {
      self->base.triggeredBy = unit;
      return 1;
    }
  }

  for (attack = (BtlAttack *)g_btlAttackList; attack != NULL;
       attack = (BtlAttack *)attack->base.next) {
    attackSq = BtlAttackGetRadius(attack);
    attackSq = attackSq * attackSq;
    if (BtlAttackUsesLinkedTarget(attack) == 0) {
      distSq = MineDistSq(attack->pos, self->base.base.pos);
    } else if (attack->effect2 != NULL) {
      distSq = MineDistSq(((GfxEffect *)attack->effect2)->pos, self->base.base.pos);
    } else {
      continue;
    }
    if (distSq <= attackSq + triggerSq) {
      self->base.triggeredBy = attack->owner;
      return 1;
    }
  }
  return 0;
}
