// bdc 0x088b0cf0 ActorStageObjPropDetectContact
#include "bdc.h"

/* Contact test of a knock-over prop. Returns 0 while a battle demo runs (`BtlIsDemoRunning`) or
   outside a battle (`BtlGetBakuganList` NULL). Otherwise returns 1 for the first unit of the
   Bakugan list whose position lies within distance `radius + combat.stats->reachRadius` of the
   prop, copying the unit's `pos`/`velocity` into `contactPos`/`contactVel` and storing the unit in
   `triggeredBy`; else for the first attack of `g_btlAttackList` whose position (linked-target
   attacks, `BtlAttackUsesLinkedTarget`: the position of `effect2`, skipped when that is NULL)
   lies within `radius + ` `BtlAttackGetRadius`, copying the attack's own `pos`/`vel` and storing
   its `owner`. Returns 0 when nothing is that close. */

static float PropDist(const float *other, const float *self)
{
  float dx = other[0] - self[0];
  float dy = other[1] - self[1];
  float dz = other[2] - self[2];

  return __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
}

static void PropCopyVec4(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

int ActorStageObjPropDetectContact(ActorStageObjProp *self)
{
  BtlBakugan **list;
  BtlBakugan *unit;
  BtlAttack *attack;
  float radius;
  float reach;
  float attackRadius;
  float dist;

  list = BtlGetBakuganList();
  if (BtlIsDemoRunning() || list == NULL) {
    return 0;
  }
  unit = *list;
  radius = self->radius;
  for (; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
    reach = unit->combat.stats->reachRadius;
    dist = PropDist(unit->base.pos, self->base.base.pos);
    if (dist <= radius + reach) {
      PropCopyVec4(self->contactPos, unit->base.pos);
      PropCopyVec4(self->contactVel, unit->base.velocity);
      self->base.triggeredBy = unit;
      return 1;
    }
  }

  for (attack = (BtlAttack *)g_btlAttackList; attack != NULL;
       attack = (BtlAttack *)attack->base.next) {
    attackRadius = BtlAttackGetRadius(attack);
    if (BtlAttackUsesLinkedTarget(attack) == 0) {
      dist = PropDist(attack->pos, self->base.base.pos);
    } else if (attack->effect2 != NULL) {
      dist = PropDist(((GfxEffect *)attack->effect2)->pos, self->base.base.pos);
    } else {
      continue;
    }
    if (dist <= attackRadius + radius) {
      PropCopyVec4(self->contactPos, attack->pos);
      PropCopyVec4(self->contactVel, attack->vel);
      self->base.triggeredBy = attack->owner;
      return 1;
    }
  }
  return 0;
}
