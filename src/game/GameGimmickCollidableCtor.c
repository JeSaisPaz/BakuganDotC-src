// bdc 0x088d5264 GameGimmickCollidableCtor
#include "bdc.h"

/* Constructor of the breakable solid gimmick: `GameGimmickCtor`, vtables
   `g_gameGimmickCollidableVtbl` / `g_gameGimmickCollidableVtbl2`, inline sphere shape
   (`shapeType = 3`, `g_collisionSphereVtbl`), `state = 0`, fog colour/near/far cleared,
   `lighting = 1`, fog colour = `g_colorBlack` packed to RGBA8 (saturated, x 255),
   specular (0.6, 0.6, 0.6, 1) power 10, root-matrix translation = `pos` with w = 1.
   The mesh collider `<model base name>_col.ctc` (from `g_gameGimmickKindTable` `[kind][0]`;
   the record's `variant` nibble, 0 unless < 5, is passed to a `sprintf` whose format has no
   conversion, so it is unused) goes into a 0x190-byte low-heap collider (`CollisionColliderCtor`
   type 3, NULL on failure) on layer 0x13 as `attached`: `byte104` and `hitField144` cleared,
   `owner = obj`, attached to the root matrix.
   A second collider (type 1) becomes `hitCollider`: sphere `shapePos` = 0 with
   w = 8² = 64, then `shapePos` = `pos` 10 up, `shapeParam` (radius) 8, recalculated through the
   shape's slot 9 virtual, layer 0xb, `byte104 = 0`. Its sphere centre is `pos` 7 up (the
   rotated 3-unit offset along pi/2 - `rot[1]` is computed, then its x and z are zeroed), refreshed
   through slot 9 and copied to `hitPos`. `broken = 0`, three 0x38-byte aligned copies of the
   record in `recordCopy`, motion speed 0 (vtable slot 6), `pos.y += 7`,
   `GameGimmickEnableMaterialCallback`. `noEffect` is stored; when it is 0 and
   `g_gameEventFlags[0]` and `[2]` are both 0, effect 0x56 is spawned on `g_worldEffectMgr`
   attached to `effectPos` (root translation, 11 up) and `effectActive = 1`. Returns `obj`. */

CoreObject *GameGimmickCollidableCtor(GameGimmickCollidable *obj, s32 kind, void *record, u16 typeId, u8 flag,
                                      s32 noEffect)
{
  CollisionCollider *collider;
  CollisionCollider *mem;
  CollisionSphere *sphere;
  GfxEffect *effect;
  const VtblEntry *e;
  GmoModel *model;
  GameGimmickRecord *rec;
  void *mesh;
  void *copy;
  char *dot;
  bool fromLow;
  u32 packed;
  s32 variant;
  s32 i;
  float specular[4] __attribute__((aligned(16)));
  char name[128];
  char suffix[32];
  float shapeTmp[4] __attribute__((aligned(16)));
  float center[4] __attribute__((aligned(16)));

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickCollidableVtbl;
  obj->base.vtbl2 = g_gameGimmickCollidableVtbl2;
  obj->shapeVtbl = g_collisionSphereVtbl;
  obj->shapeType = 3;
  variant = 0;
  obj->base.state = 0;
  obj->base.base.fogColor = 0;
  obj->base.base.fogFar = 0.0f;
  obj->base.base.fogNear = 0.0f;
  obj->base.base.lighting = 1;
  /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q */
  packed = (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.x) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.y) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.z) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.w) * 255.0f, 23)) << 24;
  obj->base.base.fogColor = packed;

  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(10.0f, &obj->base.base, specular, NULL);

  rec = (GameGimmickRecord *)obj->base.record;
  if ((rec->variant & 0xf) < 5) {
    variant = ((GameGimmickRecord *)obj->base.record)->variant & 0xf;
  }

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
  sprintf(suffix, "_col.ctc", variant);
  strcat(name, suffix);

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
  CollisionColliderInitMesh((CoreNode *)collider, mesh, 0x13, obj, 0);

  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  ((CollisionCollider *)obj->base.attached)->hitField144 = 0;
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
  obj->hitCollider = (CoreNode *)collider;

  /* sv.q C720: the bank's zero vector */
  obj->shapePos.x = 0.0f;
  obj->shapePos.y = 0.0f;
  obj->shapePos.z = 0.0f;
  obj->shapePos.w = 0.0f;
  obj->shapeParam = 8.0f;
  obj->shapePos.w = 8.0f * 8.0f;
  /* shapePos = pos + (0, 10, 0, 0) */
  shapeTmp[0] = obj->base.base.pos[0];
  shapeTmp[1] = obj->base.base.pos[1];
  shapeTmp[2] = obj->base.base.pos[2];
  shapeTmp[3] = obj->base.base.pos[3];
  shapeTmp[1] = shapeTmp[1] + 10.0f;
  obj->shapePos.x = shapeTmp[0];
  obj->shapePos.y = shapeTmp[1];
  obj->shapePos.z = shapeTmp[2];
  obj->shapePos.w = shapeTmp[3];
  e = &obj->shapeVtbl[9];
  ((void (*)(void *))e->fn)((u8 *)&obj->shapeType + e->delta);
  CollisionColliderInit(obj->hitCollider, (const u32 *)&obj->shapeType, 0xb, obj, 0);
  ((CollisionCollider *)obj->hitCollider)->byte104 = 0;

  /* center.xyz = (pos.x, pos.y + 7, pos.z); offset = 3 * (cos, 0, sin)(pi/2 - rot[1]) with x and
     z then zeroed; center.w is never written (uninitialised stack word, copied as is) */
  center[0] = obj->base.base.pos[0];
  center[1] = obj->base.base.pos[1] + 7.0f;
  center[2] = obj->base.base.pos[2];
  /* offset = 3 * (cos, 0, sin, 0)(pi/2 - rot[1]) (vrot [C,0,S,0], vscl.t by 3), then x and z are
     zeroed: every lane added (vadd.t) is 0, so only the +0.0f adds remain */
  center[0] = center[0] + 0.0f;
  center[1] = center[1] + 0.0f;
  center[2] = center[2] + 0.0f;
  sphere = (CollisionSphere *)((CollisionCollider *)obj->hitCollider)->shapeDesc;
  /* sphere centre and radius² slot = center (sv.q; radiusSq gets the uninitialised lane) */
  sphere->center[0] = center[0];
  sphere->center[1] = center[1];
  sphere->center[2] = center[2];
  sphere->radiusSq = center[3];
  e = &sphere->vtbl[9];
  ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);
  obj->hitPos.x = center[0];
  obj->hitPos.y = center[1];
  obj->hitPos.z = center[2];
  obj->hitPos.w = center[3];
  obj->broken = 0;

  for (i = 0; i < 3; i++) {
    copy = MemAllocAligned(sizeof(GameGimmickRecord), true);
    obj->recordCopy[i] = copy;
    memcpy(copy, obj->base.record, sizeof(GameGimmickRecord));
  }

  e = &((const VtblEntry *)obj->base.base.base.vtable)[6];
  ((float (*)(void *, float))e->fn)((u8 *)obj + e->delta, 0.0f);
  obj->base.base.pos[1] = obj->base.base.pos[1] + 7.0f;
  GameGimmickEnableMaterialCallback(&obj->base);

  obj->effectActive = 0;
  obj->noEffect = noEffect;
  if (noEffect == 0 && g_gameEventFlags[0] == 0 && g_gameEventFlags[2] == 0) {
    model = obj->base.base.data;
    obj->effectPos.x = model->rootMatrix[12];
    obj->effectPos.y = model->rootMatrix[13];
    obj->effectPos.z = model->rootMatrix[14];
    obj->effectPos.w = model->rootMatrix[15];
    effect = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0x56, &obj->base.base.data->rootMatrix[12]);
    effect->textureSlot = 4;
    effect->vec1d0[1] = 4.0f;
    effect->size[0] = 1.0f;
    effect->size[1] = 1.0f;
    effect->size[2] = 1.0f;
    effect->size[3] = 0.0f;
    obj->effectPos.y = obj->effectPos.y + 11.0f;
    effect->attachPos = &obj->effectPos.x;
    obj->effectActive = 1;
  }
  return &obj->base.base.base;
}
