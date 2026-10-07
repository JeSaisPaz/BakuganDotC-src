// bdc 0x088daaec GameGimmickSolidCtor
#include "bdc.h"

/* Constructor of a simple solid gimmick (touch panel): `GameGimmickCtor`, vtables
   `g_gameGimmickSolidVtbl` / `g_gameGimmickSolidVtbl2`, `state = 0`, fog colour/near/far
   cleared, `lighting = 1`, fog colour = `g_colorBlack` packed to RGBA8,
   specular (0.6, 0.6, 0.6, 1) power 10 (`GfxModelSetSpecular`), root-matrix translation = `pos`
   with w = 1. Then allocates a 0x190-byte collider from the low heap (`CollisionColliderCtor`
   type 3, NULL on failure) into `attached`, gives it the mesh `fz_quest_touchpanel01_hit.ctc` from
   `g_ioLzsPackages` on layer 9 (`CollisionColliderInitMesh`), clears `byte104`, `hitField144`
   and flag bit 1, sets `owner = obj` and attaches it to the root matrix. Returns `obj`. */

CoreObject *GameGimmickSolidCtor(GameGimmick *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  CollisionCollider *collider;
  CollisionCollider *mem;
  GmoModel *model;
  void *mesh;
  bool fromLow;
  float specular[4] __attribute__((aligned(16)));
  char name[30] = "fz_quest_touchpanel01_hit.ctc";

  GameGimmickCtor(obj, kind, record, typeId, flag);
  obj->base.base.vtable = g_gameGimmickSolidVtbl;
  obj->vtbl2 = g_gameGimmickSolidVtbl2;
  obj->state = 0;
  obj->base.fogColor = 0;
  obj->base.fogFar = 0.0f;
  obj->base.fogNear = 0.0f;
  obj->base.lighting = 1;
  obj->base.fogColor = (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.x) * 255.0f, 23)) |
      (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.y) * 255.0f, 23)) << 8 |
      (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.z) * 255.0f, 23)) << 16 |
      (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.w) * 255.0f, 23)) << 24;

  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(10.0f, &obj->base, specular, NULL);

  model = obj->base.data;
  model->rootMatrix[12] = obj->base.pos[0];
  model->rootMatrix[13] = obj->base.pos[1];
  model->rootMatrix[14] = obj->base.pos[2];
  model->rootMatrix[15] = obj->base.pos[3];
  obj->base.data->rootMatrix[15] = 1.0f;

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
  obj->attached = collider;
  mesh = CorePackChainFind(g_ioLzsPackages, name);
  CollisionColliderInitMesh((CoreNode *)collider, mesh, 9, obj, 0);

  ((CollisionCollider *)obj->attached)->byte104 = 0;
  ((CollisionCollider *)obj->attached)->hitField144 = 0;
  ((CollisionCollider *)obj->attached)->flags &= ~2u;
  ((CollisionCollider *)obj->attached)->owner = obj;
  model = obj->base.data;
  collider = (CollisionCollider *)obj->attached;
  collider->attachMatrix = (float (*)[4])model->rootMatrix;
  collider->attachDirty = 1;
  return &obj->base.base;
}
