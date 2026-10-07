// bdc 0x088d7ebc GameGimmickCameraCtor
#include "bdc.h"

/* Constructor of the surveillance-camera gimmick (`EW_GMKOBJ_SCAM`, NPC_VXS_003.gmo):
   `GameGimmickCtor`, vtables `g_gameGimmickCameraVtbl` / `g_gameGimmickCameraVtbl2`, cone
   and spot visible, root-matrix translation = `pos` with w = 1. Allocates a 0x190-byte collider
   from the low heap (`CollisionColliderCtor` type 2, NULL on failure) into `attached`, gives it
   the mesh `g_gameGimmickCameraColName` (`NPC_VXS_003_col.ctc`) from `g_ioLzsPackages` on
   layer 9, clears `byte104`/`hitField144`, sets `owner = obj` and attaches it to the root matrix.
   Enables motion when `GmoMotionMgrExists`, creates a 1-slot sound object, clears `state`,
   `step`, `sweepTimer`, `decay = 1`, finds the lens node `NPC_VXS_003_03` (its +0x60 vec4 becomes
   `lensPos`), `coneAngle = pi/6`, `sweepAngle = 0`, `coneMatrix = identity`, effects NULL. With a
   lens node, `coneMatrix = rootMatrix * node->localMatrix`, the cone effect 0x2f follows it
   (`GfxEffectSpawnFollowMatrix`) and the spot effect 0x31 (`GfxEffectCreate`) is placed at the
   root translation raised by 0.1. `mode = record->variant & 0xf`, 0 when not below 3. Installs the
   material callback and returns `obj`. */

CoreObject *GameGimmickCameraCtor(GameGimmickCamera *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  CollisionCollider *collider;
  CollisionCollider *mem;
  GmoModel *model;
  GmoNode *node;
  GfxEffect *spot;
  GameGimmickRecord *rec = (GameGimmickRecord *)record;
  ScePspFMatrix4 *cone;
  void *mesh;
  bool fromLow;
  s32 mode;
  float *dst;
  s32 i;
  s32 j;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickCameraVtbl;
  obj->base.vtbl2 = g_gameGimmickCameraVtbl2;
  obj->coneVisible = 1;
  obj->spotVisible = 1;
  model = obj->base.base.data;
  model->rootMatrix[12] = obj->base.base.pos[0];
  model->rootMatrix[13] = obj->base.base.pos[1];
  model->rootMatrix[14] = obj->base.base.pos[2];
  model->rootMatrix[15] = obj->base.base.pos[3];
  obj->base.base.data->rootMatrix[15] = 1.0f;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  cone = &obj->coneMatrix;
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 2);
    collider = mem;
  }
  obj->base.attached = collider;
  mesh = CorePackChainFind(g_ioLzsPackages, (char *)g_gameGimmickCameraColName);
  CollisionColliderInitMesh((CoreNode *)collider, mesh, 9, obj, 0);

  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  ((CollisionCollider *)obj->base.attached)->hitField144 = 0;
  ((CollisionCollider *)obj->base.attached)->owner = obj;
  model = obj->base.base.data;
  collider = (CollisionCollider *)obj->base.attached;
  collider->attachMatrix = (float (*)[4])model->rootMatrix;
  collider->attachDirty = 1;
  if (GmoMotionMgrExists()) {
    GfxModelEnableMotion(&obj->base.base);
  }
  GfxModelCreateSoundObject(&obj->base.base, 1);
  obj->base.state = 0;
  obj->step = 0;
  obj->decay = 1.0f;
  obj->sweepTimer = 0;
  obj->lensNode = GfxModelFindNode(&obj->base.base, "NPC_VXS_003_03");
  if (obj->lensNode != NULL) {
    /* +0x60 is GmoNode.rotate in the struct definition; here it is read as the lens position */
    node = (GmoNode *)obj->lensNode;
    obj->lensPos.x = node->rotate[0];
    obj->lensPos.y = node->rotate[1];
    obj->lensPos.z = node->rotate[2];
    obj->lensPos.w = node->rotate[3];
  }
  obj->coneAngle = 0.5235988f;
  obj->sweepAngle = 0.0f;
  cone->x.x = 1.0f;
  cone->x.y = 0.0f;
  cone->x.z = 0.0f;
  cone->x.w = 0.0f;
  cone->y.x = 0.0f;
  cone->y.y = 1.0f;
  cone->y.z = 0.0f;
  cone->y.w = 0.0f;
  cone->z.x = 0.0f;
  cone->z.y = 0.0f;
  cone->z.z = 1.0f;
  cone->z.w = 0.0f;
  cone->w.x = 0.0f;
  cone->w.y = 0.0f;
  cone->w.z = 0.0f;
  cone->w.w = 1.0f;
  obj->coneEffect = NULL;
  obj->spotEffect = NULL;
  if (obj->lensNode != NULL) {
    model = obj->base.base.data;
    node = (GmoNode *)obj->lensNode;
    /* vmmul.q M000, M100, M200 with M100 = rootMatrix, M200 = node->localMatrix (fields as columns):
       field j, lane i = sum_k root[i][k] * local[j][k]. */
    dst = (float *)cone;
    for (j = 0; j < 4; j++) {
      for (i = 0; i < 4; i++) {
        dst[j * 4 + i] = model->rootMatrix[i * 4 + 0] * node->localMatrix[j * 4 + 0] +
                         model->rootMatrix[i * 4 + 1] * node->localMatrix[j * 4 + 1] +
                         model->rootMatrix[i * 4 + 2] * node->localMatrix[j * 4 + 2] +
                         model->rootMatrix[i * 4 + 3] * node->localMatrix[j * 4 + 3];
      }
    }
    obj->coneEffect = GfxEffectSpawnFollowMatrix(g_worldEffectMgr, 0x2f, (float *)cone);
    obj->spotEffect = GfxEffectCreate(g_worldEffectMgr, 0x31);
    if (obj->spotEffect != NULL) {
      spot = (GfxEffect *)obj->spotEffect;
      model = obj->base.base.data;
      spot->pos[0] = model->rootMatrix[12];
      spot->pos[1] = model->rootMatrix[13];
      spot->pos[2] = model->rootMatrix[14];
      spot->pos[3] = model->rootMatrix[15];
      spot = (GfxEffect *)obj->spotEffect;
      spot->pos[1] = spot->pos[1] + 0.1f;
    }
  }
  mode = rec->variant & 0xf;
  obj->mode = mode;
  if (mode < 0 || mode >= 3) {
    obj->mode = 0;
  }
  GameGimmickEnableMaterialCallback(&obj->base);
  return &obj->base.base.base;
}
