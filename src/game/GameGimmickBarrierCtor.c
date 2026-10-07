// bdc 0x088d98c0 GameGimmickBarrierCtor
#include "bdc.h"

/* Constructor of the barrier gimmick (`GMKOBJ_BARRIER_01`, model `gfx_102m.gmo`):
   `GameGimmickCtor`, vtables `g_gameGimmickBarrierVtbl` / `g_gameGimmickBarrierVtbl2`,
   `state = 0`, fog colour/near/far cleared, `lighting = 1`, fog colour = `g_colorBlack` packed
   to RGBA8, specular (0, 0, 0, 1) power 10, material 0 texture slot 2,
   `ambient[3] = 0.999`, root-matrix translation = `pos` with w = 1. Node 1 of the model gets the
   record extents (20.12) as the diagonal of its +0x30 matrix and of its `localMatrix`. The
   collision is field-specific: `<model base name>_col_f<area>_<block>_<record byte +0x28>.ctc`
   (area/block = `g_gameEventFlags` [0]/[2]), loaded from `g_ioLzsPackages` into a 0x190-byte
   low-heap collider (`CollisionColliderCtor` type 2, NULL on failure) on layer 0x19; clears
   `byte104`/`hitField144`, sets `owner = obj` and attaches it to the root matrix. Then
   `GameGimmickBarrierBindNode`, `pulseTimer = 0`, `pulseDown = 0`; returns `obj`. */

CoreObject *GameGimmickBarrierCtor(GameGimmickBarrier *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  CollisionCollider *collider;
  CollisionCollider *mem;
  GfxMaterialState *matState;
  GameGimmickRecord *rec;
  GmoModel *model;
  GmoNode *node;
  float *diag;
  void *mesh;
  char *dot;
  bool fromLow;
  float specular[4] __attribute__((aligned(16)));
  char name[128];
  char suffix[36];

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickBarrierVtbl;
  obj->base.vtbl2 = g_gameGimmickBarrierVtbl2;
  obj->base.state = 0;
  obj->base.base.fogColor = 0;
  obj->base.base.fogFar = 0.0f;
  obj->base.base.fogNear = 0.0f;
  obj->base.base.lighting = 1;
  obj->base.base.fogColor = (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.x) * 255.0f, 23)) |
      (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.y) * 255.0f, 23)) << 8 |
      (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.z) * 255.0f, 23)) << 16 |
      (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.w) * 255.0f, 23)) << 24;

  specular[0] = 0.0f;
  specular[1] = 0.0f;
  specular[2] = 0.0f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(10.0f, &obj->base.base, specular, NULL);
  matState = (GfxMaterialState *)GfxModelGetMaterialState(&obj->base.base, 0);
  matState->texSlot = 2;
  obj->base.base.ambient[3] = 0.999f;

  model = obj->base.base.data;
  model->rootMatrix[12] = obj->base.base.pos[0];
  model->rootMatrix[13] = obj->base.base.pos[1];
  model->rootMatrix[14] = obj->base.base.pos[2];
  model->rootMatrix[15] = obj->base.base.pos[3];
  obj->base.base.data->rootMatrix[15] = 1.0f;

  /* The barrier treats node +0x30..+0x6f as a 4x4 matrix (GmoNode definition: `matrix` pointer at
     +0x30) and writes its diagonal (+0x30, +0x44, +0x58). */
  node = GfxModelGetNode(&obj->base.base, 1);
  diag = (float *)&node->matrix;
  rec = (GameGimmickRecord *)obj->base.record;
  diag[0] = (float)rec->extent[0] * (1.0f / 4096.0f);
  diag[5] = (float)rec->extent[1] * (1.0f / 4096.0f);
  diag[10] = (float)rec->extent[2] * (1.0f / 4096.0f);
  node = GfxModelGetNode(&obj->base.base, 1);
  rec = (GameGimmickRecord *)obj->base.record;
  node->localMatrix[0] = (float)rec->extent[0] * (1.0f / 4096.0f);
  node->localMatrix[5] = (float)rec->extent[1] * (1.0f / 4096.0f);
  node->localMatrix[10] = (float)rec->extent[2] * (1.0f / 4096.0f);

  strcpy(name, g_gameGimmickKindTable[kind][0]);
  dot = strrchr(name, '.');
  if (dot != NULL) {
    *dot = '\0';
  }
  rec = (GameGimmickRecord *)obj->base.record;
  sprintf(suffix, "_col_f%d_%02d_%02d.ctc", g_gameEventFlags[0], g_gameEventFlags[2],
          rec->barrierColIndex);
  strcat(name, suffix);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 2);
    collider = mem;
  }
  obj->base.attached = collider;
  mesh = CorePackChainFind(g_ioLzsPackages, name);
  CollisionColliderInitMesh((CoreNode *)collider, mesh, 0x19, obj, 0);

  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  ((CollisionCollider *)obj->base.attached)->hitField144 = 0;
  ((CollisionCollider *)obj->base.attached)->owner = obj;
  model = obj->base.base.data;
  collider = (CollisionCollider *)obj->base.attached;
  collider->attachMatrix = (float (*)[4])model->rootMatrix;
  collider->attachDirty = 1;
  GameGimmickBarrierBindNode(obj);
  obj->pulseTimer = 0;
  obj->pulseDown = 0;
  return &obj->base.base.base;
}
