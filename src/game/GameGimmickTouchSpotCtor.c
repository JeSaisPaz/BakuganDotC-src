// bdc 0x088dafc0 GameGimmickTouchSpotCtor
#include "bdc.h"

/* Constructor of a touch-trigger gimmick: `GameGimmickCtor`, vtables
   `g_gameGimmickTouchSpotVtbl`/`g_gameGimmickTouchSpotVtbl2`, state 0, model root translation =
   object position, ambient alpha 0, a sphere shape descriptor at `+0x190` (type 3,
   `g_collisionSphereVtbl`, centre 0 (the bank's zero column C720), radius 3, radius² 9), a
   `CollisionColliderCtor` collider (type 1) on layer 9 built from it whose sphere centre (and
   radius² slot) is set to the model root translation and recalculated (`CollisionSphereRecalc`
   through the vtable), and effect 8 at the object position (`GfxEffectSpawn` on
   `g_worldEffectMgr`, handle `+0x180`). Returns `obj`. */

CoreObject *GameGimmickTouchSpotCtor(GameGimmickTouchSpot *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  bool fromLow;
  CollisionCollider *collider;
  CollisionSphere *sphere;
  const VtblEntry *e;
  float *root;
  float radius;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickTouchSpotVtbl;
  obj->base.vtbl2 = g_gameGimmickTouchSpotVtbl2;
  obj->shapeVtbl = g_collisionSphereVtbl;
  obj->shapeType = 3;
  obj->base.state = 0;
  /* model root translation = object position */
  root = &obj->base.base.data->rootMatrix[12];
  root[0] = obj->base.base.pos[0];
  root[1] = obj->base.base.pos[1];
  root[2] = obj->base.base.pos[2];
  root[3] = obj->base.base.pos[3];
  obj->base.base.ambient[3] = 0.0f;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  collider = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (collider != NULL) {
    CollisionColliderCtor(&collider->node, 1);
  }
  obj->base.attached = collider;

  /* sphere centre/w = bank zero column C720 */
  obj->shapePos.x = 0.0f;
  obj->shapePos.y = 0.0f;
  obj->shapePos.z = 0.0f;
  obj->shapePos.w = 0.0f;
  radius = 3.0f;
  obj->shapeParam = radius;
  obj->shapePos.w = radius * radius;
  CollisionColliderInit((CoreNode *)obj->base.attached, (const u32 *)&obj->shapeType, 9, obj, 0);

  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  sphere = (CollisionSphere *)((CollisionCollider *)obj->base.attached)->shapeDesc;
  /* sphere centre and radius² slot = model root translation (x, y, z, w) */
  root = &obj->base.base.data->rootMatrix[12];
  sphere->center[0] = root[0];
  sphere->center[1] = root[1];
  sphere->center[2] = root[2];
  sphere->radiusSq = root[3];
  e = &sphere->vtbl[9];
  ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);

  obj->sprite = GfxEffectSpawn(g_worldEffectMgr, 8, obj->base.base.pos);
  return (CoreObject *)obj;
}
