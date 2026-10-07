// bdc 0x088a0180 GameGimmickCorePointCtor
#include "bdc.h"

/* Constructor of the core-point (repair point) gimmicks, kinds 0xa..0xf
   (`EW_GMKOBJ_COREPOINT_S/M/L` and the invisible variants, models
   `btl_06_repairpoint01`..`btl_17_repairpoint06`): `GameGimmickCtor`, vtables
   `g_gameGimmickCorePointVtbl` / `g_gameGimmickCorePointVtbl2`, state/node/timer/spin cleared,
   `cpKind = kind`, `invisible` from bit 0x10 of the record's `visFlags`, a sphere shape descriptor
   at `+0x1b0` (type 3, `g_collisionSphereVtbl`), the hop Bezier (`CoreBezierCtor`) and the hop
   vectors / `scale` zeroed (VFPU bank C720). Root matrix = identity with translation
   `pos`; `groundY` = ground under `pos` (`CollisionRaycastPointB`) + 2, `pos.y += 8`, `baseY = pos.y`.
   Motion speed 1 (vtable slot 6) and looping. By `kind - 10`: 0 `core01` scale 10, 1 `core14` 12,
   2 `RootNode` 14, 3/4/5 `RootNode` with the blend bits 0–1 of byte `+3` of the materials
   `btl_06_repairpoint01`/`02` cleared and scale 10/12/14 (other kinds: no node name, scale 0).
   Lighting on; a 0x190-byte collider from the low heap (`CollisionColliderCtor` type 1, NULL on
   failure) in `attached`, initialised on layer 9 from the sphere (centre/w = 0, radius 3,
   radius² 9, `CollisionColliderInit`); its sphere centre is set to `pos` and recalculated
   (vtable slot 9). `pickupRadius` = 0.8 * the XZ length of the model bbox extent
   (`CollisionAabbExtent`); every material gets blend mode 1
   (`GameGimmickCorePointMaterialSetBlendMode1`); flags cleared. Returns `obj`.
   C720 and S713 are VFPU bank constants (0). */

CoreObject *GameGimmickCorePointCtor(GameGimmickCorePoint *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  static const ScePspFVector4 zero = {0.0f, 0.0f, 0.0f, 0.0f};
  ScePspFVector4 ground __attribute__((aligned(16)));
  ScePspFVector4 extent __attribute__((aligned(16)));
  GameGimmickRecord *rec = (GameGimmickRecord *)record;
  const VtblEntry *e;
  CollisionCollider *mem;
  CollisionCollider *collider;
  CollisionSphere *sphere;
  u8 *mat;
  float *m;
  s32 i;
  bool fromLow;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickCorePointVtbl;
  obj->base.vtbl2 = g_gameGimmickCorePointVtbl2;
  obj->cpState = 0;
  obj->coreNode = NULL;
  obj->timer = 0;
  obj->baseY = 0.0f;
  obj->spin = 0;
  obj->groundY = 0.0f;
  obj->cpKind = kind;
  obj->coreScale = 0.0f;
  obj->invisible = (rec->visFlags & 0x10) != 0;
  obj->shapeVtbl = g_collisionSphereVtbl;
  obj->shapeType = 3;
  CoreBezierCtor(&obj->hop);
  /* C720 bank constant = (0, 0, 0, 0) */
  obj->hopStart = zero;
  obj->hopMid = zero;
  obj->hopEnd = zero;
  obj->scale = zero;
  obj->hopStep = zero;
  obj->hopHeight = 0.0f;
  obj->buffer = NULL;

  /* root matrix = identity, translation = pos; ground = pos */
  m = obj->base.base.data->rootMatrix;
  for (i = 0; i < 16; i++)
    m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  m = &obj->base.base.data->rootMatrix[12];
  m[0] = obj->base.base.pos[0];
  m[1] = obj->base.base.pos[1];
  m[2] = obj->base.base.pos[2];
  m[3] = obj->base.base.pos[3];
  ground.x = obj->base.base.pos[0];
  ground.y = obj->base.base.pos[1];
  ground.z = obj->base.base.pos[2];
  ground.w = obj->base.base.pos[3];
  CollisionRaycastPointB(&ground.x, &ground.x);
  obj->groundY = ground.y + 2.0f;
  obj->base.base.pos[1] = obj->base.base.pos[1] + 8.0f;
  obj->baseY = obj->base.base.pos[1];

  e = &((const VtblEntry *)obj->base.base.base.vtable)[6];
  ((float (*)(void *, float))e->fn)((u8 *)obj + e->delta, 1.0f);
  GfxModelSetMotionLoop(&obj->base.base, 1);

  switch (obj->cpKind - 10) {
  case 0:
    sprintf(obj->coreNodeName, "core01");
    obj->coreScale = 10.0f;
    break;
  case 1:
    sprintf(obj->coreNodeName, "core14");
    obj->coreScale = 12.0f;
    break;
  case 2:
    sprintf(obj->coreNodeName, "RootNode");
    obj->coreScale = 14.0f;
    break;
  case 3:
    sprintf(obj->coreNodeName, "RootNode");
    mat = GfxModelFindMaterialState(&obj->base.base, "btl_06_repairpoint01");
    mat[3] &= ~3;
    mat = GfxModelFindMaterialState(&obj->base.base, "btl_06_repairpoint02");
    mat[3] &= ~3;
    obj->coreScale = 10.0f;
    break;
  case 4:
    sprintf(obj->coreNodeName, "RootNode");
    mat = GfxModelFindMaterialState(&obj->base.base, "btl_06_repairpoint01");
    mat[3] &= ~3;
    mat = GfxModelFindMaterialState(&obj->base.base, "btl_06_repairpoint02");
    mat[3] &= ~3;
    obj->coreScale = 12.0f;
    break;
  case 5:
    sprintf(obj->coreNodeName, "RootNode");
    mat = GfxModelFindMaterialState(&obj->base.base, "btl_06_repairpoint01");
    mat[3] &= ~3;
    mat = GfxModelFindMaterialState(&obj->base.base, "btl_06_repairpoint02");
    mat[3] &= ~3;
    obj->coreScale = 14.0f;
    break;
  }
  obj->base.base.lighting = 1;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x190, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 1);
    collider = mem;
  }
  obj->base.attached = collider;

  /* sphere centre/w = C720 bank constant (0), radius 3, radius² 9 */
  obj->shapePos = zero;
  obj->shapeParam = 3.0f;
  obj->shapePos.w = 3.0f * 3.0f;
  CollisionColliderInit((CoreNode *)obj->base.attached, (const u32 *)&obj->shapeType, 9, obj, 0);
  ((CollisionCollider *)obj->base.attached)->byte104 = 0;
  sphere = (CollisionSphere *)((CollisionCollider *)obj->base.attached)->shapeDesc;
  /* 16-byte copy: pos.w lands in radiusSq, recalculated by slot 9 */
  sphere->center[0] = obj->base.base.pos[0];
  sphere->center[1] = obj->base.base.pos[1];
  sphere->center[2] = obj->base.base.pos[2];
  sphere->radiusSq = obj->base.base.pos[3];
  e = &sphere->vtbl[9];
  ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);

  /* pickupRadius = |0.8 * extent| over x/z (the y lane is zeroed before the dot) */
  CollisionAabbExtent(obj->base.base.data->bbox, &extent);
  extent.x = extent.x * 0.8f;
  extent.y = extent.y * 0.8f;
  extent.z = extent.z * 0.8f;
  extent.w = 0.0f; /* S713 bank constant */
  obj->pickupRadius = __builtin_sqrtf(extent.x * extent.x + 0.0f * 0.0f + extent.z * extent.z);

  GfxModelForEachMaterial(&obj->base.base, GameGimmickCorePointMaterialSetBlendMode1, NULL);
  obj->translucent = 0;
  obj->opaque = 0;
  obj->launched = 0;
  obj->effectAttached = 0;
  obj->hopCount = 0;
  return (CoreObject *)obj;
}
