// bdc 0x088d6e18 GameGimmickItemBoxCtor
#include "bdc.h"

/* Constructor of the item-box gimmick (`EW_GMKOBJ_ITEMBOX`, fz_quest_obstacle01.gmo):
   `GameGimmickCtor`, vtables `g_gameGimmickItemBoxVtbl` / `g_gameGimmickItemBoxVtbl2`.
   Scales the record vector `+0x28` (20.12) by 20 into C710 (stored only to a dead stack
   temporary), puts the position into the root matrix translation (w = 1) and snapshots the root
   matrix into `colliderMatrix`. Allocates a collider (type 2, NULL on failure) into `attached`,
   gives it the mesh `g_itemBoxColliderName` (`fz_quest_obstacle01.ctc`) on layer 9 and attaches
   it to `colliderMatrix`. Rotates the root matrix about X by record `+0x28` and then about Z by
   record `+0x30` (angles 20.12 radians; each rotation pre-multiplies the matrix), calls vtable slot 6 (motion speed) with 0.0f,
   disables motion looping, reads the motion frame, clears `state` and `step`, sets `unusedSlot = -1`.
   The first item box creates the shared break-effect models `g_itemBoxBreakEffect1` /
   `g_itemBoxBreakEffect2` (`fz_quest_obstacle01_ef01.gmo` / `_ef02.gmo`, NULL on allocation
   failure); every one increments `g_itemBoxCount`. Sets `breakAlpha = 1.0f`, enables the
   material callback and returns `obj`. */

CoreObject *GameGimmickItemBoxCtor(GameGimmickItemBox *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  GameGimmickRecord *rec;
  CollisionCollider *mem;
  CollisionCollider *collider;
  GfxModel *model;
  GfxModel *effect;
  const VtblEntry *entry;
  void *mesh;
  bool fromLow;
  float *m;
  float scaled[4];
  float tmp[4];
  float angX;
  float angZ;
  float c;
  float sn;
  float x;
  float y;
  float z;
  int i;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickItemBoxVtbl;
  obj->base.vtbl2 = g_gameGimmickItemBoxVtbl2;

  rec = (GameGimmickRecord *)obj->base.record;
  scaled[0] = (float)rec->jetDoorVec[0] * 0.000244140625f;
  scaled[1] = (float)rec->jetDoorVec[1] * 0.000244140625f;
  scaled[2] = (float)rec->jetDoorVec[2] * 0.000244140625f;
  scaled[3] = 0.0f;
  /* dead stack temporary: xyz * 20, w = bank S713 (0) */
  tmp[0] = scaled[0] * 20.0f;
  tmp[1] = scaled[1] * 20.0f;
  tmp[2] = scaled[2] * 20.0f;
  tmp[3] = 0.0f;
  (void)tmp;

  m = obj->base.base.data->rootMatrix;
  m[12] = obj->base.base.pos[0];
  m[13] = obj->base.base.pos[1];
  m[14] = obj->base.base.pos[2];
  m[15] = obj->base.base.pos[3];
  obj->base.base.data->rootMatrix[15] = 1.0f;
  obj->colliderMatrix = *(const ScePspFMatrix4 *)obj->base.base.data->rootMatrix;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x190, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 2);
    collider = mem;
  }
  obj->base.attached = collider;
  mesh = CorePackChainFind(g_ioLzsPackages, g_itemBoxColliderName);
  CollisionColliderInitMesh((CoreNode *)collider, mesh, 9, obj, 0);

  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  ((CollisionCollider *)obj->base.attached)->hitField144 = 0;
  ((CollisionCollider *)obj->base.attached)->owner = obj;
  collider = (CollisionCollider *)obj->base.attached;
  collider->attachMatrix = (float (*)[4])&obj->colliderMatrix;
  collider->attachDirty = 1;

  m = obj->base.base.data->rootMatrix;
  rec = (GameGimmickRecord *)record;
  angX = (float)rec->jetDoorVec[0] * 0.000244140625f;
  angZ = (float)rec->jetDoorVec[2] * 0.000244140625f;

  /* root = Rx(angX) * root (`vrot` of angX * 2/pi quarter turns, `vmmul.q E200, E100, E000`):
     each column (x, y, z, w) becomes (x, c*y - s*z, s*y + c*z, w). */
  c = __builtin_cosf(angX);
  sn = __builtin_sinf(angX);
  for (i = 0; i < 4; i++) {
    y = m[i * 4 + 1];
    z = m[i * 4 + 2];
    m[i * 4 + 1] = c * y - sn * z;
    m[i * 4 + 2] = sn * y + c * z;
  }

  /* root = Rz(angZ) * root: each column becomes (c*x - s*y, s*x + c*y, z, w). */
  c = __builtin_cosf(angZ);
  sn = __builtin_sinf(angZ);
  for (i = 0; i < 4; i++) {
    x = m[i * 4 + 0];
    y = m[i * 4 + 1];
    m[i * 4 + 0] = c * x - sn * y;
    m[i * 4 + 1] = sn * x + c * y;
  }

  entry = &((const VtblEntry *)obj->base.base.base.vtable)[6];
  ((float (*)(void *, float))entry->fn)((u8 *)obj + entry->delta, 0.0f);
  GfxModelSetMotionLoop(&obj->base.base, 0);
  GfxModelMotionFrame(&obj->base.base);
  obj->base.state = 0;
  obj->step = 0;
  obj->unusedSlot = -1;

  if (g_itemBoxCount == 0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    model = MemAlloc(0x140, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    effect = NULL;
    if (model != NULL) {
      GfxModelCtor(model, "fz_quest_obstacle01_ef01.gmo", 0);
      effect = model;
    }
    g_itemBoxBreakEffect1 = effect;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    model = MemAlloc(0x140, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    effect = NULL;
    if (model != NULL) {
      GfxModelCtor(model, "fz_quest_obstacle01_ef02.gmo", 0);
      effect = model;
    }
    g_itemBoxBreakEffect2 = effect;
  }
  g_itemBoxCount++;
  obj->breakAlpha = 1.0f;
  GameGimmickEnableMaterialCallback(&obj->base);
  return &obj->base.base.base;
}
