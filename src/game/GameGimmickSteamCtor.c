// bdc 0x088d5ccc GameGimmickSteamCtor
#include "bdc.h"

/* Constructor of the steam gimmick (`EW_GMKOBJ_STEAM`, model Steam_Wall.gmo): `GameGimmickCtor`,
   vtables `g_gameGimmickSteamVtbl`/`g_gameGimmickSteamVtbl2`, timing
   (`GameGimmickSteamSetTiming`), start timer from `g_gameGimmickSteamStartTimers` by the low
   nibble of the record's `variant`, model root translation = object position (w = 1), and a mesh
   collider (type 2) from `Steam_Wall.ctc` on layer 9 that follows the root matrix and starts
   disabled (flag 4, steam off). Returns `obj`. */

CoreObject *GameGimmickSteamCtor(GameGimmickSteam *obj, s32 kind, void *record, u16 typeId, u8 flag)

{
  const GameGimmickRecord *rec;
  bool fromLow;
  CollisionCollider *collider;
  void *mesh;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickSteamVtbl;
  obj->base.vtbl2 = g_gameGimmickSteamVtbl2;
  GameGimmickSteamSetTiming(obj);
  rec = (const GameGimmickRecord *)obj->base.record;
  obj->timer = g_gameGimmickSteamStartTimers[rec->variant & 0xf];
  /* model root translation = object position (w then forced to 1) */
  obj->base.base.data->rootMatrix[12] = obj->base.base.pos[0];
  obj->base.base.data->rootMatrix[13] = obj->base.base.pos[1];
  obj->base.base.data->rootMatrix[14] = obj->base.base.pos[2];
  obj->base.base.data->rootMatrix[15] = obj->base.base.pos[3];
  obj->base.base.data->rootMatrix[15] = 1.0f;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  collider = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (collider != NULL) {
    CollisionColliderCtor(&collider->node, 2);
  }
  obj->base.attached = collider;
  mesh = CorePackChainFind(g_ioLzsPackages, "Steam_Wall.ctc");
  CollisionColliderInitMesh((CoreNode *)collider, mesh, 9, obj, 0);

  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  ((CollisionCollider *)obj->base.attached)->hitField144 = 0;
  ((CollisionCollider *)obj->base.attached)->owner = obj;
  collider = (CollisionCollider *)obj->base.attached;
  collider->attachMatrix = (float (*)[4])obj->base.base.data->rootMatrix;
  collider->attachDirty = 1;
  ((CollisionCollider *)obj->base.attached)->flags |= 4;
  obj->venting = 0;
  return (CoreObject *)obj;
}
