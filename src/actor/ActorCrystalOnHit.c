// bdc 0x0885a0f0 ActorCrystalOnHit
#include "bdc.h"

/* Hit-reaction virtual of the crystal actor (vtable `0x08af1974` slot `+0xc0`, overriding
   `BtlBakuganOnHit`). Does nothing while virtual entry 22 reports true. The attack id is the
   collider's `hitKind`, the hit class its `hitParam164`. The heading from the crystal to the hit
   point (`atan2f`) is computed on both paths.
   When `ActorCrystalCanBeHitBy` accepts the hit class, the hit is bounced without damage: a
   direction from the hit point (or, for linked-target attack ids (`BtlHitIdUsesLinkedTarget`),
   the attacker's position, which also replaces the heading) toward the anchor point 200 units up
   is built; within 800 units of the crystal it is flattened, normalised and blended 15% toward the
   camera direction and the spawn point is 150 units out along the heading from the hit point, else
   the direction's Y is scaled by 0.2 and the point is 270 units out from the anchor; the direction
   is normalised, effect 0x37 (`GfxEffectSpawnDirected`) and sound `0x20006f` play there, the
   collider gets `flags |= 1`, `hitTimer = 1`, `cooldown = 0` and its hit position moved to that
   point.
   Otherwise the hit lands: hit spark (`BtlBakuganSpawnHitSpark`), break sound `0x20001f` at the
   model translation (`SndEmitterCreateAtPos`), hit effect 4 units toward the camera
   (`ActorCrystalSpawnHitEffect`); in rule mode 2 the attacker's score/hit counters are raised
   (class 1: `comboDamage` or profile word `14 + playerSlot`; class 2: hit tally via
   `BtlBakuganAddClampedHitCount` or the same profile word; doubled when time runs out). Then
   `BtlCombatTakeHit` applies the damage; if entry 22 now reports true the HP is set to
   `maxHp * hpThreshold`. A valid attacker gets 0 combo hits, a combo timer (15 or 0 for state-9
   attackers in some kinds/motions) and `linkedUnit = self`; if the crystal died, its
   `crystalBreakCount` is raised and, in rule mode 2, profile word `14 + playerSlot` gains 20
   (40 when time runs out). Always ends with `shaking = 1`, `shakeFrame = 0` and
   `knockdownGauge++`. */

/* v.xyz = normalise(v.xyz) saturated to [-1, 1] (a zero vector stays zero); v.w = 0 (bank S713) */
static inline void ActorCrystalOnHitNormalize(float *v)
{
  float len2;
  float k;

  len2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
  v[0] = VfSat1(v[0] * k);
  v[1] = VfSat1(v[1] * k);
  v[2] = VfSat1(v[2] * k);
  v[3] = 0.0f;
}

/* out = {cos(heading) * radius + base.x, 0 + base.y, sin(heading) * radius + base.z, 0} */
static inline void ActorCrystalOnHitPolar(float *out, float heading, float radius, const float *base)
{
  out[0] = __builtin_cosf(heading) * radius + base[0];
  out[1] = 0.0f * radius + base[1];
  out[2] = __builtin_sinf(heading) * radius + base[2];
  out[3] = 0.0f;
}

void ActorCrystalOnHit(ActorCrystal *self)
{
  const VtblEntry *entry;
  CollisionCollider *collider;
  BtlBakugan *attacker;
  BtlBakugan *found;
  void *linked;
  s32 attackId;
  s32 canBounce;
  s32 hitClass;
  s32 points;
  s32 slot;
  s32 kind;
  s32 maxHp;
  u32 word;
  float threshold;
  float heading;
  float dist;
  float dx, dy, dz;
  float spawn[4];
  float dir[4];
  float tmp[4];
  float hit[4];
  float anchor[4];
  const float *cam;
  int i;

  attackId = self->base.collider0->hitKind;
  attacker = NULL;
  entry = &((const VtblEntry *)self->base.base.base.vtable)[22];
  if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
    return;
  }
  canBounce = ActorCrystalCanBeHitBy(self, self->base.collider0->hitParam164);
  collider = self->base.collider0;
  heading = atan2f(collider->hitPos.z - self->base.base.pos[2],
                   collider->hitPos.x - self->base.base.pos[0]);
  if (canBounce) {
    for (i = 0; i < 4; i++) {
      anchor[i] = self->base.anchorMatrix[3][i];
    }
    collider = self->base.collider0;
    hit[0] = collider->hitPos.x;
    hit[1] = collider->hitPos.y;
    hit[2] = collider->hitPos.z;
    hit[3] = collider->hitPos.w;
    if (BtlHitIdUsesLinkedTarget(attackId) != 0) {
      linked = self->base.collider0->hitAttacker;
      if (linked != NULL) {
        for (i = 0; i < 4; i++) {
          hit[i] = ((BtlBakugan *)linked)->base.pos[i];
        }
      }
      heading = atan2f(hit[2] - self->base.base.pos[2], hit[0] - self->base.base.pos[0]);
    }
    anchor[1] = anchor[1] + 200.0f;
    /* dir = {anchor.xyz - hit.xyz, anchor.w} */
    tmp[0] = anchor[0] - hit[0];
    tmp[1] = anchor[1] - hit[1];
    tmp[2] = anchor[2] - hit[2];
    tmp[3] = anchor[3];
    for (i = 0; i < 4; i++) {
      dir[i] = tmp[i];
    }
    dx = self->base.base.pos[0] - hit[0];
    dy = self->base.base.pos[1] - hit[1];
    dz = self->base.base.pos[2] - hit[2];
    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist <= 800.0f) {
      dir[1] = 0.0f;
      ActorCrystalOnHitNormalize(dir);
      /* dir += (camera dir - dir) * 0.15f, all four components */
      cam = g_gfxActiveCamera->dir;
      for (i = 0; i < 4; i++) {
        dir[i] = dir[i] + (cam[i] - dir[i]) * 0.150000006f;
      }
      ActorCrystalOnHitPolar(spawn, heading, 150.0f, &self->base.collider0->hitPos.x);
      spawn[1] = spawn[1] + 100.0f;
    }
    else {
      dir[1] = dir[1] * 0.2f;
      ActorCrystalOnHitPolar(spawn, heading, 270.0f, self->base.anchorMatrix[3]);
      spawn[1] = (self->base.collider0->hitPos.y + 100.0f) + spawn[1];
    }
    ActorCrystalOnHitNormalize(dir);
    GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x37, spawn, dir);
    BtlBakuganPlaySound(&self->base, 0x20006f, 0, 0);
    collider = self->base.collider0;
    collider->flags = collider->flags | 1;
    collider->hitTimer = 1;
    self->base.collider0->cooldown = 0;
    collider = self->base.collider0;
    collider->hitPos.x = spawn[0];
    collider->hitPos.y = spawn[1];
    collider->hitPos.z = spawn[2];
    collider->hitPos.w = spawn[3];
    return;
  }

  BtlBakuganSpawnHitSpark(&self->base, 1, BtlBakuganListFind(attacker), attackId);
  if (SndHasListener()) {
    SndEmitterCreateAtPos(SndGetListener(), 0x20001f, &self->base.base.data->rootMatrix[12], 0, 1);
  }
  /* spawn = {hit point.xyz - camera dir.xyz * 4.0f, hit point.w};
     dir = {spawn.xyz - pos.xyz, spawn.w} */
  collider = self->base.collider0;
  cam = g_gfxActiveCamera->dir;
  spawn[0] = collider->hitPos.x - cam[0] * 4.0f;
  spawn[1] = collider->hitPos.y - cam[1] * 4.0f;
  spawn[2] = collider->hitPos.z - cam[2] * 4.0f;
  spawn[3] = collider->hitPos.w;
  dir[0] = spawn[0] - self->base.base.pos[0];
  dir[1] = 0.0f;
  dir[2] = spawn[2] - self->base.base.pos[2];
  dir[3] = spawn[3];
  ActorCrystalOnHitNormalize(dir);
  ActorCrystalSpawnHitEffect(self, spawn, dir);

  hitClass = self->base.collider0->hitParam164;
  if (hitClass < 2) {
    if (hitClass > 0) {
      attacker = self->base.collider0->hitAttacker;
      if (g_scriptGlobalVars[8] == 2) {
        points = 1;
        if (BtlIsTimeRunningOut()) {
          points = 2;
        }
        if (SaveProfileGetWord(SaveGetProfile(), 7) == 1) {
          slot = attacker->playerSlot + 14;
          if (attacker->state == 9 || attacker->state == 0xb ||
              BtlAttackIdUsesHitCounter(attacker, attackId) != 0) {
            if (attacker->comboDamage < points * 5) {
              attacker->comboDamage = attacker->comboDamage + points;
            }
          }
          else if (BtlCameraTaskExists() != 0) {
            BtlGetCameraTask();
            if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
              SaveProfileAddWord(SaveGetProfile(), slot, points);
            }
          }
        }
      }
    }
  }
  else if (hitClass < 3) {
    attacker = self->base.collider0->hitAttacker;
    points = 1;
    if (BtlIsTimeRunningOut()) {
      points = 2;
    }
    word = SaveProfileGetWord(SaveGetProfile(), 7);
    if ((s32)word < 2 && (s32)word > 0) {
      slot = attacker->playerSlot + 14;
      if ((attacker->stateFlags & 0x80000) != 0 ||
          BtlAttackIdUsesHitCounter(attacker, attackId) != 0 || attackId == 0x5a ||
          attackId == 0x59) {
        if (attackId == 0x5a || attackId == 0x59) {
          if (attacker->hitTally < points) {
            BtlBakuganAddClampedHitCount(attacker, points, points);
          }
        }
        else if (attacker->hitTally < points * 5) {
          BtlBakuganAddClampedHitCount(attacker, points, points);
        }
      }
      else if (BtlCameraTaskExists() != 0) {
        BtlGetCameraTask();
        if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
          SaveProfileAddWord(SaveGetProfile(), slot, points);
        }
      }
    }
  }

  found = BtlBakuganListFind(attacker);
  BtlCombatTakeHit(&self->base.combat, found, hitClass, attackId, 0, 0, 6);
  entry = &((const VtblEntry *)self->base.base.base.vtable)[22];
  if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
    threshold = self->base.hpThreshold;
    maxHp = BtlCombatGetMaxHp(&self->base.combat);
    BtlCombatSetHp((float)(u32)maxHp * threshold, &self->base.combat);
  }
  if (found != NULL) {
    BtlBakuganAddComboHits(found, 0);
    if (found->state == 9) {
      kind = (s32)found->base.base.unk08;
      if (kind == 5 || kind == 6 || kind == 0x11) {
        found->comboTimer = 15;
      }
      else if (BtlBakuganIsMotion(found, 0x94) != 0 || BtlBakuganIsMotion(found, 0xa1) != 0 ||
               BtlBakuganIsMotion(found, 0x95) != 0 || BtlBakuganIsMotion(found, 0xa0) != 0) {
        if (kind == 0xd) {
          found->comboTimer = 15;
        }
        else {
          found->comboTimer = 0;
        }
      }
    }
    found->linkedUnit = self;
    if (self->base.combat.dead != 0) {
      found->crystalBreakCount = found->crystalBreakCount + 1;
      if (g_scriptGlobalVars[8] == 2) {
        points = 20;
        if (BtlIsTimeRunningOut()) {
          points = 40;
        }
        if (SaveProfileGetWord(SaveGetProfile(), 7) == 1) {
          slot = found->playerSlot + 14;
          if (BtlCameraTaskExists() != 0) {
            BtlGetCameraTask();
            if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
              SaveProfileAddWord(SaveGetProfile(), slot, points);
            }
          }
        }
      }
    }
  }
  self->shaking = 1;
  self->shakeFrame = 0;
  self->base.knockdownGauge = self->base.knockdownGauge + 1;
}
