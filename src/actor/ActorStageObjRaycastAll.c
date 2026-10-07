// bdc 0x088ab6dc ActorStageObjRaycastAll
#include "bdc.h"

/* Casts the segment `origin` + `delta` (copied into `g_collisionSegmentDesc`) against every
   standing (`dead` clear), non-pushable stage object in `g_actorStageObjList`: objects with their
   own collision box (`buffer`) through `CollisionBoxTestSegmentQuery` on it, the others as a
   type-6 oriented box (`g_collisionBoxVtbl`) built from `ActorStageObjGetBounds` and the
   model's root matrix; then, when a battle is running, against the units of `BtlGetBakuganList`
   (combat `dead` clear and vtable slot 11 non-zero; box from their model's bbox and root matrix).
   Keeps the nearest hit distance from `origin` (to `g_collisionHitResult`) in `*t` (starts at
   `g_actorStageObjRaycastInf`, +inf) and returns the kind of the nearest hit: 0 when there is no
   Bakugan list (outside a battle), 3 when a unit was the nearest hit, else from the nearest stage
   object's category: 1 for categories 0/1/2/4, 4 for 5/6, 2 for 8, 0 for 3/7, any other category
   or no hit. Called by `BtlAiRaycastBlocked`. */

/* Copies the 4x4 matrix `src` (16 floats) into `dst`. */
static inline void ActorStageObjRaycastCopyMatrix(ScePspFMatrix4 *dst, const float *src)
{
  dst->x.x = src[0];
  dst->x.y = src[1];
  dst->x.z = src[2];
  dst->x.w = src[3];
  dst->y.x = src[4];
  dst->y.y = src[5];
  dst->y.z = src[6];
  dst->y.w = src[7];
  dst->z.x = src[8];
  dst->z.y = src[9];
  dst->z.z = src[10];
  dst->z.w = src[11];
  dst->w.x = src[12];
  dst->w.y = src[13];
  dst->w.z = src[14];
  dst->w.w = src[15];
}

/* Builds the cached inverse of `box->transform` (3x3 transpose with `w` 0, translation the negated
   rotated translation, `w.w` kept), as the inlined `CollisionBoxTestSegmentQuery` helper does. */
static inline void ActorStageObjRaycastBoxInvert(CollisionBox *box)
{
  const ScePspFMatrix4 *t;
  ScePspFMatrix4 *inv;
  float px;
  float py;
  float pz;

  if (box->invValid == 0) {
    t = &box->transform;
    inv = &box->invTransform;
    px = t->x.x * t->w.x + t->x.y * t->w.y + t->x.z * t->w.z;
    py = t->y.x * t->w.x + t->y.y * t->w.y + t->y.z * t->w.z;
    pz = t->z.x * t->w.x + t->z.y * t->w.y + t->z.z * t->w.z;
    inv->x.x = t->x.x;
    inv->x.y = t->y.x;
    inv->x.z = t->z.x;
    inv->x.w = 0.0f;
    inv->y.x = t->x.y;
    inv->y.y = t->y.y;
    inv->y.z = t->z.y;
    inv->y.w = 0.0f;
    inv->z.x = t->x.z;
    inv->z.y = t->y.z;
    inv->z.z = t->z.z;
    inv->z.w = 0.0f;
    inv->w.x = -px;
    inv->w.y = -py;
    inv->w.z = -pz;
    inv->w.w = t->w.w;
    box->invValid = 1;
  }
}

/* Distance (xyz) from `origin` to the last hit point `g_collisionHitResult`. */
static inline float ActorStageObjRaycastHitDist(const float *origin)
{
  float dx;
  float dy;
  float dz;

  dx = origin[0] - g_collisionHitResult.point.x;
  dy = origin[1] - g_collisionHitResult.point.y;
  dz = origin[2] - g_collisionHitResult.point.z;
  return __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
}

s32 ActorStageObjRaycastAll(float *origin, float *delta, float *t)
{
  CollisionBox objBox __attribute__((aligned(16)));
  CollisionBox unitBox __attribute__((aligned(16)));
  ActorStageObjBase *obj;
  ActorStageObjBase *nearestObj;
  BtlBakugan **list;
  BtlBakugan *unit;
  BtlBakugan *nearestUnit;
  const VtblEntry *vtbl;
  GmoModel *model;
  float *bounds;
  float best;
  float dist;
  s32 kind;
  s32 hit;

  kind = 0;
  g_collisionSegmentDesc.start[0] = origin[0];
  g_collisionSegmentDesc.start[1] = origin[1];
  g_collisionSegmentDesc.start[2] = origin[2];
  g_collisionSegmentDesc.start[3] = origin[3];
  g_collisionSegmentDesc.dir[0] = delta[0];
  g_collisionSegmentDesc.dir[1] = delta[1];
  g_collisionSegmentDesc.dir[2] = delta[2];
  g_collisionSegmentDesc.dir[3] = delta[3];
  best = g_actorStageObjRaycastInf;
  *t = best;
  nearestObj = NULL;
  if (g_actorStageObjList != NULL) {
    for (obj = (ActorStageObjBase *)g_actorStageObjList->head; obj != NULL;
         obj = (ActorStageObjBase *)obj->base.base.next) {
      if (obj->dead != 0) {
        continue;
      }
      if (ActorStageObjIsPushable(obj) != 0) {
        continue;
      }
      hit = 0;
      if (obj->buffer != NULL) {
        if (CollisionBoxTestSegmentQuery((CollisionBox *)obj->buffer, &g_collisionSegmentDesc)) {
          hit = 1;
        }
      } else {
        objBox.vtbl = g_collisionBoxVtbl;
        objBox.invValid = 0;
        objBox.type = 6;
        bounds = ActorStageObjGetBounds(obj);
        model = obj->base.data;
        objBox.aabbMin = *(ScePspFVector4 *)&bounds[0];
        objBox.aabbMax = *(ScePspFVector4 *)&bounds[4];
        objBox.invValid = 0;
        ActorStageObjRaycastCopyMatrix(&objBox.transform, model->rootMatrix);
        ActorStageObjRaycastBoxInvert(&objBox);
        if (CollisionBoxTestSegmentQuery(&objBox, &g_collisionSegmentDesc)) {
          hit = 1;
        }
      }
      if (hit == 0) {
        continue;
      }
      dist = ActorStageObjRaycastHitDist(origin);
      if (!(best <= dist)) {
        best = dist;
        nearestObj = obj;
      }
    }
  }
  list = (BtlBakugan **)BtlGetBakuganList();
  if (list == NULL) {
    *t = best;
    return 0;
  }
  nearestUnit = NULL;
  for (unit = *list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
    if (unit->combat.dead != 0) {
      continue;
    }
    vtbl = (const VtblEntry *)unit->base.base.vtable;
    if (((s32 (*)(void *))vtbl[11].fn)((u8 *)unit + vtbl[11].delta) == 0) {
      continue;
    }
    hit = 0;
    unitBox.vtbl = g_collisionBoxVtbl;
    unitBox.invValid = 0;
    unitBox.type = 6;
    model = unit->base.data;
    unitBox.aabbMin = *(ScePspFVector4 *)&model->bbox[0];
    unitBox.aabbMax = *(ScePspFVector4 *)&model->bbox[4];
    unitBox.invValid = 0;
    ActorStageObjRaycastCopyMatrix(&unitBox.transform, model->rootMatrix);
    ActorStageObjRaycastBoxInvert(&unitBox);
    if (CollisionBoxTestSegmentQuery(&unitBox, &g_collisionSegmentDesc)) {
      hit = 1;
    }
    if (hit == 0) {
      continue;
    }
    dist = ActorStageObjRaycastHitDist(origin);
    if (!(best < dist)) {
      best = dist;
      nearestUnit = unit;
    }
  }
  if (nearestUnit != NULL) {
    kind = 3;
  } else if (nearestObj != NULL) {
    switch ((u32)nearestObj->category) {
    case 0:
    case 1:
    case 2:
    case 4:
      kind = 1;
      break;
    case 5:
    case 6:
      kind = 4;
      break;
    case 8:
      kind = 2;
      break;
    case 3:
    case 7:
    default:
      break;
    }
  }
  *t = best;
  return kind;
}
