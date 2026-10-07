// bdc 0x088db4ac GameGimmickSwitchCtor
#include "bdc.h"

/* Constructor of the floor-switch gimmick (`GMKOBJ_SWITCH_01`, model `fz_quest_switch01.gmo`):
   `GameGimmickCtor`, vtables `g_gameGimmickSwitchVtbl` / `g_gameGimmickSwitchVtbl2`, inline
   sphere shape (`shapeType = 3`, `g_collisionSphereVtbl`), `state = 0`, fog colour/near/far
   cleared, `lighting = 1`, fog colour = `g_colorBlack` packed to RGBA8,
   specular (0.6, 0.6, 0.6, 1) power 10, root-matrix translation = `pos` with w = 1.
   The mesh collider `<model base name>_col.ctc` (from `g_gameGimmickKindTable` `[kind][0]`,
   loaded from `g_ioLzsPackages`) goes into a 0x190-byte low-heap collider
   (`CollisionColliderCtor` type 3, NULL on failure) on layer 9 as `attached`: `byte104` and
   `hitField144` cleared, flag bit 1 cleared, `owner = obj`, attached to the root matrix.
   A second collider (type 1) becomes `pressCollider`: sphere `shapePos` = (0, 0, 0) with
   w = 3² = 9, `shapeParam` (radius) 3, layer 9, `byte104 = 0`; its centre is the position 14 up
   plus 3 along (cos, 0, sin) of pi/2 - `rot[1]`, refreshed through the shape's recalc virtual
   (slot 9) and copied to `pressPos`. Then `GameGimmickSwitchBindGlowNode` and
   `GameGimmickSwitchSetPose` with `active != 0`; returns `obj`. */

CoreObject *GameGimmickSwitchCtor(GameGimmickSwitch *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  CollisionCollider *collider;
  CollisionCollider *mem;
  CollisionSphere *sphere;
  const VtblEntry *e;
  GmoModel *model;
  void *mesh;
  char *dot;
  bool fromLow;
  float angle;
  float specular[4];
  char name[128];
  float center[3];
  float offset[3];

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickSwitchVtbl;
  obj->base.vtbl2 = g_gameGimmickSwitchVtbl2;
  obj->shapeVtbl = g_collisionSphereVtbl;
  obj->shapeType = 3;
  obj->base.state = 0;
  obj->base.base.fogColor = 0;
  obj->base.base.fogFar = 0.0f;
  obj->base.base.fogNear = 0.0f;
  obj->base.base.lighting = 1;
  obj->base.base.fogColor = (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.x) * 255.0f, 23)) |
                           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.y) * 255.0f, 23)) << 8 |
                           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.z) * 255.0f, 23)) << 16 |
                           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.w) * 255.0f, 23)) << 24;

  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(10.0f, &obj->base.base, specular, NULL);

  model = obj->base.base.data;
  model->rootMatrix[12] = obj->base.base.pos[0];
  model->rootMatrix[13] = obj->base.base.pos[1];
  model->rootMatrix[14] = obj->base.base.pos[2];
  model->rootMatrix[15] = obj->base.base.pos[3];
  obj->base.base.data->rootMatrix[15] = 1.0f;

  strcpy(name, g_gameGimmickKindTable[kind][0]);
  dot = strrchr(name, '.');
  if (dot != NULL) {
    *dot = '\0';
  }
  strcat(name, "_col.ctc");

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 3);
    collider = mem;
  }
  obj->base.attached = collider;
  mesh = CorePackChainFind(g_ioLzsPackages, name);
  CollisionColliderInitMesh((CoreNode *)collider, mesh, 9, obj, 0);

  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  ((CollisionCollider *)obj->base.attached)->hitField144 = 0;
  ((CollisionCollider *)obj->base.attached)->flags &= ~2u;
  ((CollisionCollider *)obj->base.attached)->owner = obj;
  model = obj->base.base.data;
  collider = (CollisionCollider *)obj->base.attached;
  collider->attachMatrix = (float (*)[4])model->rootMatrix;
  collider->attachDirty = 1;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 1);
    collider = mem;
  }
  obj->pressCollider = (CoreNode *)collider;

  obj->shapePos.x = 0.0f;
  obj->shapePos.y = 0.0f;
  obj->shapePos.z = 0.0f;
  obj->shapePos.w = 0.0f;
  obj->shapeParam = 3.0f;
  obj->shapePos.w = 3.0f * 3.0f;
  CollisionColliderInit(obj->pressCollider, (const u32 *)&obj->shapeType, 9, obj, 0);
  ((CollisionCollider *)obj->pressCollider)->byte104 = 0;

  /* center.xyz = (pos.x, pos.y + 14, pos.z) + 3 * (cos, 0, sin)(pi/2 - rot[1]); center.w is
     never written (an uninitialised stack word that the asm copies into the sphere's radiusSq,
     overwritten by the recalc, and into pressPos.w): left out here */
  center[0] = obj->base.base.pos[0];
  center[1] = obj->base.base.pos[1] + 14.0f;
  center[2] = obj->base.base.pos[2];
  angle = 1.5707964f - obj->base.base.rot[1];
  offset[0] = __builtin_cosf(angle) * 3.0f;
  offset[1] = 0.0f * 3.0f;
  offset[2] = __builtin_sinf(angle) * 3.0f;
  center[0] = center[0] + offset[0];
  center[1] = center[1] + offset[1];
  center[2] = center[2] + offset[2];
  sphere = (CollisionSphere *)((CollisionCollider *)obj->pressCollider)->shapeDesc;
  sphere->center[0] = center[0];
  sphere->center[1] = center[1];
  sphere->center[2] = center[2];
  e = &sphere->vtbl[9];
  ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);
  obj->pressPos.x = center[0];
  obj->pressPos.y = center[1];
  obj->pressPos.z = center[2];

  GameGimmickSwitchBindGlowNode(obj);
  GameGimmickSwitchSetPose(obj, obj->base.active != 0);
  return &obj->base.base.base;
}
