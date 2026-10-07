// bdc 0x0881dbe0 GfxEffectHitTest
#include "bdc.h"

/* Effect command that tests the `effect`'s hit volume against the world colliders:
   fills the static query `g_gfxEffectHitQuery` (its `contact` zeroed once from `g_gfxVecZero`) –
   a sphere of radius `radius` at the effect position (`shape` 0, `g_btlBakuganSphereQuery`) or a
   swept sphere (`g_collisionSweptSphereDesc`) along a direction of length `length` (`shape` 1–3 =
   x/y/z, 4 = the effect's `dir` scaled to `length`) – with the owner Bakugan `ownerBakugan` (if still
   in the list; its `collider0` is excluded), the attack kind `kind`, its class
   (`GfxEffectAttackHitClass`) and `param4` as flags, and runs `CollisionHitQuery` (layer mask
   `0x31bf337e`, or `0x3e` for kinds 0x1a–0x1f). The hit flag goes to `g_gfxEffectHitFlag`; for kinds
   0x23..0xb2 with an owner, a hit also queues the hit (`kind − 0x23`) on the hit collider's unit
   (`BtlBakuganQueueHit`). For kinds 0x1a–0x1f the flag is kept only for a hit collider of layer 1–4
   whose owner returns 0 from vtable slot 11 and whose slot 20 kind is not `kind − 0x1a`; for all other
   kinds it is cleared again before returning.
   The local direction starts as the zero vector (bank constant C720); for `shape` 4 a zero `dir`
   gives a zero direction, and its `w` is the bank's S713 (0). */

void GfxEffectHitTest(float radius, float length, GfxEffect *effect, s32 param4, s32 kind, u32 shape)
{
  ScePspFVector4 sweep;
  ScePspFVector4 dir;
  float lenSq;
  float scale;
  CollisionQuery *query;
  CollisionSweptSphereDesc *desc;
  CollisionShapeBlock *block;
  CollisionCollider *col;
  BtlBakugan *unit;
  const VtblEntry *vtbl;
  void *exclude;
  u32 layerMask;
  s32 isSoftKind;
  s32 stat;

  query = &g_gfxEffectHitQuery;
  sweep.x = 0.0f;
  sweep.y = 0.0f;
  sweep.z = 0.0f;
  sweep.w = 0.0f;
  if (shape < 5) {
    if (shape == 1) {
      sweep.x = length;
    } else if (shape == 2) {
      sweep.y = length;
    } else if (shape == 3) {
      sweep.z = length;
    } else if (shape == 4) {
      /* sweep = normalize(dir.xyz) * length (zero when dir is zero), w = 0 */
      dir.x = effect->dir[0];
      dir.y = effect->dir[1];
      dir.z = effect->dir[2];
      lenSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
      scale = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
      scale = scale * length;
      sweep.x = dir.x * scale;
      sweep.y = dir.y * scale;
      sweep.z = dir.z * scale;
      sweep.w = 0.0f;
    }
  }
  if (g_gfxEffectHitQueryInit == 0) {
    g_gfxEffectHitQueryInit = 1;
    query->contact = g_gfxVecZero;
  }
  if (shape == 0) {
    g_btlBakuganSphereQuery.radius = radius;
    g_btlBakuganSphereQuery.center.x = effect->pos[0];
    g_btlBakuganSphereQuery.center.y = effect->pos[1];
    g_btlBakuganSphereQuery.center.z = effect->pos[2];
    g_btlBakuganSphereQuery.center.w = effect->pos[3];
    g_btlBakuganSphereQuery.center.w = g_btlBakuganSphereQuery.radius * g_btlBakuganSphereQuery.radius;
    query->shape = &g_collisionSphereBlock;
  } else {
    desc = &g_collisionSweptSphereDesc;
    desc->radius = radius;
    desc->start.x = effect->pos[0];
    desc->start.y = effect->pos[1];
    desc->start.z = effect->pos[2];
    desc->start.w = effect->pos[3];
    desc->dir = sweep;
    desc->start.w = desc->radius * desc->radius;
    /* dir.w = |dir.xyz| */
    desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y +
                                  desc->dir.z * desc->dir.z);
    block = (CollisionShapeBlock *)desc->shapeBlock;
    query->shape = block;
    desc = (CollisionSweptSphereDesc *)block->shape;
    desc->start.x = effect->pos[0];
    desc->start.y = effect->pos[1];
    desc->start.z = effect->pos[2];
    desc->start.w = effect->pos[3];
  }

  layerMask = 0x31bf337e;
  isSoftKind = 0;
  if (kind >= 0x1a && kind < 0x20) {
    layerMask = 0x3e;
    isSoftKind = 1;
  }
  exclude = NULL;
  if (effect->ownerBakugan != NULL && BtlBakuganListFind((BtlBakugan *)effect->ownerBakugan) != NULL) {
    unit = (BtlBakugan *)effect->ownerBakugan;
    exclude = unit->collider0;
    query->owner = unit;
    query->hitParam = 3;
  } else {
    query->owner = NULL;
    query->hitParam = 4;
  }
  query->ownerCollider = NULL;
  query->heading = 0.0f;
  query->attackId = kind;
  query->attackSide = GfxEffectAttackHitClass(effect, kind);
  query->flags = (u32)param4;
  g_gfxEffectHitFlag = (u8)CollisionHitQuery(layerMask, &query->contact.x, 1, exclude);

  if (g_gfxEffectHitFlag != 0 && g_collisionLastHitCollider != NULL && query->owner != NULL &&
      kind >= 0x23 && kind < 0xb3) {
    stat = kind - 0x23;
    if (stat < 0) {
      stat = 0;
    } else if (stat > 0x8d) {
      stat = 0x8d;
    }
    unit = (BtlBakugan *)g_collisionLastHitCollider->owner;
    if (BtlBakuganListFind(unit) != NULL) {
      BtlBakuganQueueHit(unit, (s16)stat, effect->ownerBakugan);
    }
  }

  if (isSoftKind == 0 || g_gfxEffectHitFlag == 0) {
    g_gfxEffectHitFlag = 0;
    return;
  }
  col = (CollisionCollider *)query->hitCollider;
  if (col == NULL) {
    g_gfxEffectHitFlag = 0;
    return;
  }
  if ((s32)col->layer <= 0 ||
      (s32)col->layer >= 5) {
    g_gfxEffectHitFlag = 0;
    return;
  }
  unit = (BtlBakugan *)col->owner;
  if (unit == NULL) {
    g_gfxEffectHitFlag = 0;
    return;
  }
  vtbl = (const VtblEntry *)unit->base.base.vtable;
  if (((s32 (*)(void *))vtbl[11].fn)((u8 *)unit + vtbl[11].delta) != 0) {
    g_gfxEffectHitFlag = 0;
    return;
  }
  vtbl = (const VtblEntry *)unit->base.base.vtable;
  if (((s32 (*)(void *))vtbl[20].fn)((u8 *)unit + vtbl[20].delta) == kind - 0x1a) {
    g_gfxEffectHitFlag = 0;
  }
}
