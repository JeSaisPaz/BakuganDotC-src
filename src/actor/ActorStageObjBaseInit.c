// bdc 0x088aad28 ActorStageObjBaseInit
#include "bdc.h"

/* Main initialisation of a stage object (`ActorStageObjBaseCtor`): object kind 0x85, appends it
   to the stage-object chain `g_actorStageObjList` (allocated on first use), default colours,
   sound parameters (`ActorStageObjKindGetSoundParams`), HP (`ActorStageObjGetKindMaxHp`),
   frame limit (30 s × fps), identity rotation matrix, hides trees
   (`ActorStageObjKindStartsHidden`), places it (`ActorStageObjPlace`) and, when it needs
   collision (`ActorStageObjNeedsCollider`) and `<model>_col.ctc` is in the pack chain, builds a
   400-byte mesh collider (`CollisionColliderCtor`, `CollisionColliderInitMesh`) at `collider`
   bound to the model matrix; sets `blocksCamera` for kinds 6, 0x3f, 0x6a, 0x6b, 0x7c, 0x85 and
   landmarks other than 0x5d, arms category-8 colliders, records the height, and for a collider
   object that does not block the camera builds a `CollisionBox` (`buffer`) from the bounds and
   model matrix with its inverse. Ends with `BtlStageApplyLightColors`.
   `base.velocity` and `hitPos` are zeroed (the VFPU bank zero C720). */

void ActorStageObjBaseInit(ActorStageObjBase *self, u32 *pos)
{
  char name[164];
  CoreObjectList *list;
  CollisionCollider *col;
  CollisionBox *box;
  float *bounds;
  float maxY;
  float hp;
  bool fromLow;
  char *dot;
  s32 kind;
  s32 i;
  s32 j;
  float *root;
  ScePspFMatrix4 *m;
  ScePspFMatrix4 *inv;
  float t0;
  float t1;
  float t2;
  float tw;

  self->base.base.unk08 = 0x85;
  self->lightCount = 0;
  if (g_actorStageObjList == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(sizeof(CoreObjectList), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_actorStageObjList = list;
    list->tail = NULL;
    g_actorStageObjList->head = NULL;
    g_actorStageObjList->count = 0;
  }
  CoreObjectListAppend(&self->base.base, g_actorStageObjList);
  self->recordState = 0;
  self->base.ambient[0] = g_colorWhite.x;
  self->base.ambient[1] = g_colorWhite.y;
  self->base.ambient[2] = g_colorWhite.z;
  self->base.ambient[3] = g_colorWhite.w;
  self->flag28a = 0;
  self->emissive[0] = g_colorBlack.x;
  self->emissive[1] = g_colorBlack.y;
  self->emissive[2] = g_colorBlack.z;
  self->emissive[3] = g_colorBlack.w;
  self->soundParams = ActorStageObjKindGetSoundParams(self->kind);
  hp = ActorStageObjGetKindMaxHp(self, self->kind);
  self->hp = (s32)hp;
  self->maxHp = (s32)hp;
  *(float *)self->spin = 0.0f;
  self->fade = 1.0f;
  self->base.velocity[0] = 0.0f;
  self->base.velocity[1] = 0.0f;
  self->base.velocity[2] = 0.0f;
  self->base.velocity[3] = 0.0f;
  self->frameCounter = 0;
  self->frameLimit = GfxDisplayGetFps(g_gfxDisplay) * 30;
  ActorStageObjIsPushable(self);
  self->flags1d0 = 0;
  self->flags1d4 = 0;
  self->step = 0;
  *(float *)self->spin = 0.0f;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      self->rotMatrix[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
  self->dead = 0;
  self->removeRequest = 0;
  *(float *)self->springY = 0.0f;
  self->val260 = 0.0f;
  self->hitPos[0] = 0.0f;
  self->hitPos[1] = 0.0f;
  self->hitPos[2] = 0.0f;
  self->hitPos[3] = 0.0f;
  self->itemDropped = 0;
  self->flag286 = 0;
  self->matrixDirty = 1;
  self->blocksCamera = 0;
  if (ActorStageObjKindStartsHidden(self, self->kind) != 0) {
    self->base.lighting = 0;
    self->base.ambient[0] = g_colorWhite.x;
    self->base.ambient[1] = g_colorWhite.y;
    self->base.ambient[2] = g_colorWhite.z;
    self->base.ambient[3] = g_colorWhite.w;
  }
  for (i = 0; i < 4; i++) {
    ((u32 *)self->base.pos)[i] = pos[i];
  }
  ActorStageObjPlace(self, (float *)pos);
  if (ActorStageObjNeedsCollider(self) == 0) {
    self->matrixDirty = 0;
  } else {
    strcpy(name, g_actorStageObjModelTable[self->kind * 3]);
    dot = strrchr(name, '.');
    if (dot != NULL) {
      *dot = '\0';
    }
    strcat(name, "_col.ctc");
    if (CorePackChainFind(g_ioLzsPackages, name) != NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      col = MemAlloc(sizeof(CollisionCollider), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (col != NULL) {
        CollisionColliderCtor((CoreNode *)col, 3);
      }
      self->collider = col;
      CollisionColliderInitMesh((CoreNode *)col, CorePackChainFind(g_ioLzsPackages, name), 9, self, 0);
      ((CollisionCollider *)self->collider)->byte104 = 0;
      ((CollisionCollider *)self->collider)->hitField144 = 1;
      ((CollisionCollider *)self->collider)->owner = self;
      col = self->collider;
      col->attachMatrix = (Mat4Row *)self->base.data->rootMatrix;
      col->attachDirty = 1;
      kind = self->kind;
      switch (kind) {
      case 6:
      case 0x3f:
      case 0x6a:
      case 0x6b:
      case 0x7c:
      case 0x85:
        self->blocksCamera = 1;
        break;
      default:
        self->blocksCamera = 0;
        break;
      }
      if (ActorStageObjIsLandmark(self) != 0 && self->kind != 0x5d) {
        self->blocksCamera = 1;
      }
    }
  }
  if (self->category == 8 && self->collider != NULL) {
    col = self->collider;
    col->hitTimer = -1;
    col->flags |= 1;
  }
  self->flag280 = 0;
  maxY = ActorStageObjGetBounds(self)[5];
  bounds = ActorStageObjGetBounds(self);
  self->height = maxY - bounds[1];
  memset(&self->shaking, 0, 0xc);
  if (self->collider != NULL && self->blocksCamera == 0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    box = MemAlloc((sizeof(CollisionBox) + 0xf) & ~0xfu, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (box != NULL) {
      box->vtbl = g_collisionBoxVtbl;
      box->invValid = 0;
      box->type = 6;
    }
    self->buffer = box;
    bounds = ActorStageObjGetBounds(self);
    box->aabbMin.x = bounds[0];
    box->aabbMin.y = bounds[1];
    box->aabbMin.z = bounds[2];
    box->aabbMin.w = bounds[3];
    box->aabbMax.x = bounds[4];
    box->aabbMax.y = bounds[5];
    box->aabbMax.z = bounds[6];
    box->aabbMax.w = bounds[7];
    box->invValid = 0;
    root = self->base.data->rootMatrix;
    box->transform.x.x = root[0];
    box->transform.x.y = root[1];
    box->transform.x.z = root[2];
    box->transform.x.w = root[3];
    box->transform.y.x = root[4];
    box->transform.y.y = root[5];
    box->transform.y.z = root[6];
    box->transform.y.w = root[7];
    box->transform.z.x = root[8];
    box->transform.z.y = root[9];
    box->transform.z.z = root[10];
    box->transform.z.w = root[11];
    box->transform.w.x = root[12];
    box->transform.w.y = root[13];
    box->transform.w.z = root[14];
    box->transform.w.w = root[15];
    box = self->buffer;
    if (box->invValid == 0) {
      /* Inverse of a rigid transform: transposed rotation, translation -(R^T * t). */
      m = &box->transform;
      inv = &box->invTransform;
      t0 = m->x.x * m->w.x + m->x.y * m->w.y + m->x.z * m->w.z;
      t1 = m->y.x * m->w.x + m->y.y * m->w.y + m->y.z * m->w.z;
      t2 = m->z.x * m->w.x + m->z.y * m->w.y + m->z.z * m->w.z;
      tw = m->w.w;
      inv->x.x = m->x.x;
      inv->x.y = m->y.x;
      inv->x.z = m->z.x;
      inv->x.w = 0.0f;
      inv->y.x = m->x.y;
      inv->y.y = m->y.y;
      inv->y.z = m->z.y;
      inv->y.w = 0.0f;
      inv->z.x = m->x.z;
      inv->z.y = m->y.z;
      inv->z.z = m->z.z;
      inv->z.w = 0.0f;
      inv->w.x = -t0;
      inv->w.y = -t1;
      inv->w.z = -t2;
      inv->w.w = tw;
      box->invValid = 1;
    }
  }
  BtlStageApplyLightColors(self);
}
