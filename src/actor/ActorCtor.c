// bdc 0x088dc644 ActorCtor
#include "bdc.h"

/* Base constructor of the in-world 3D actor class (0x3a0 bytes; Bakugan, characters, plain NPCs):
   builds the GMO model from `g_btlModelNames``[modelId]` (`GfxModelCtor`), installs the actor
   vtable `g_actorVtbl` and the vtables/types of the embedded collision shapes (capsule
   `collShape` with its segment view, sphere `bodyShape`), numbers the actor (`+0x0c` id from
   `g_actorSerial``++`, `animPhase` = `g_actorNumbering``++ % 0xdf + 0x20`, also the stencil
   ref), allocates its 0x70-byte input helper (`BtlInputCtorForActor`, `input`) and 0x30-byte
   shadow (`BtlShadowCtorForActor`, `shadow`; disabled for models 0x54/0x55, size 13), sets an
   identity matrix, packs `g_colorBlack` as the fog colour, creates a 3-slot sound object, the
   collider and the motions, appends the actor to `g_actorList` and enters state 0
   (`ActorSetStateBase`). Returns `self`.
   The cleared vectors take the VFPU bank's zero vector C720, the colour pack its 255.0 (S701). */

Actor *ActorCtor(Actor *self, s32 modelId)

{
  BtlInput *input;
  BtlShadow *shadow;
  bool fromLow;
  s32 serial;
  s32 i;

  GfxModelCtor(&self->base, g_btlModelNames[modelId], 0);
  self->base.base.vtable = &g_actorVtbl;
  self->collShape.vtbl = g_collisionCapsuleVtbl;
  ((SegmentShape *)self->collShape.segmentHead)->info = (void *)g_collisionSegmentVtbl;
  ((SegmentShape *)self->collShape.segmentHead)->type = 2;
  self->collShape.type = 4;
  self->bodyShape.vtbl = g_collisionSphereVtbl;
  self->bodyShape.type = 3;
  self->placement = NULL;
  self->routeEnds = 0;
  self->base.base.unk08 = modelId;
  self->base.base.id = g_actorSerial++;
  serial = g_actorNumbering++;
  self->animPhase = serial % 0xdf + 0x20;
  self->isPlayer = 0;
  self->walkSpeed = 0.0f;
  self->groundNormal[0] = g_vecUp.x;
  self->groundNormal[1] = g_vecUp.y;
  self->groundNormal[2] = g_vecUp.z;
  self->groundNormal[3] = g_vecUp.w;
  self->radius = 6.0f;
  /* C720: the bank's zero vector. */
  self->extraMove[0] = 0.0f;
  self->extraMove[1] = 0.0f;
  self->extraMove[2] = 0.0f;
  self->extraMove[3] = 0.0f;
  self->collisionMask = 0x3fbf2700;
  self->stepHeight = 24.0f;
  self->groundPoint[0] = 0.0f;
  self->groundPoint[1] = 0.0f;
  self->groundPoint[2] = 0.0f;
  self->groundPoint[3] = 0.0f;
  self->noGroundProbe = 0;
  self->surfaceType = 0;
  self->base.velocity[0] = 0.0f;
  self->base.velocity[1] = 0.0f;
  self->base.velocity[2] = 0.0f;
  self->base.velocity[3] = 0.0f;
  self->fallSpeed = 2.3f;
  self->cleared318 = 0;
  self->footFlags = 0;
  self->flags = 0;
  self->tiltQuat[0] = g_vecUp.x;
  self->tiltQuat[1] = g_vecUp.y;
  self->tiltQuat[2] = g_vecUp.z;
  self->tiltQuat[3] = g_vecUp.w;
  self->airFrames = 0;
  self->camera = NULL;
  self->jumpTimer = 0;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  input = MemAlloc(sizeof(BtlInput), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (input != NULL) {
    BtlInputCtorForActor(input, self);
  }
  self->input = input;
  self->motionSlots = NULL;
  self->bodyCollider = NULL;
  self->collider2 = NULL;
  /* vmidt.q: identity matrix. */
  for (i = 0; i < 16; i++) {
    self->mtx[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  shadow = MemAlloc(sizeof(BtlShadow), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (shadow != NULL) {
    BtlShadowCtorForActor(shadow, self);
  }
  self->shadow = shadow;
  if (modelId == 0x54 || modelId == 0x55) {
    ((BtlShadow *)self->shadow)->enabled = 0;
  }
  shadow = (BtlShadow *)self->shadow;
  shadow->size[0] = 13.0f;
  shadow->size[1] = 13.0f;
  shadow->size[2] = 13.0f;
  shadow->size[3] = 0.0f;

  self->stateTimer = 0;
  self->aimTargetKind = 0;
  self->stateVec[0] = 0.0f;
  self->stateVec[1] = 0.0f;
  self->aimPoint[0] = 0.0f;
  self->aimPoint[1] = 0.0f;
  self->aimPoint[2] = 0.0f;
  self->aimPoint[3] = 0.0f;
  self->stateVec300[0] = 0.0f;
  self->stateVec300[1] = 0.0f;
  self->stateVec300[2] = 0.0f;
  self->stateVec300[3] = 0.0f;
  self->spawnTag = 0;
  self->waitTimer = 0;
  self->stateStep = 0;
  self->motionOverrideTimer = 0;
  self->motionOverride = 0;
  self->stateDelay = 0;
  self->eventFlag = 0;
  self->faceExpression = -1;
  self->motionSlot = 0;
  GfxModelSetStencilRef(&self->base, (u8)self->animPhase);
  self->base.fogColor = 0;
  self->base.fogFar = 0.0f;
  self->base.fogNear = 0.0f;
  self->base.lighting = 1;
  /* vsat0.q, vscl.q by S701 (the bank's 255), vf2iz.q 23, vi2uc.q */
  self->base.fogColor = (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.x) * 255.0f, 23)) |
                        (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.y) * 255.0f, 23)) << 8 |
                        (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.z) * 255.0f, 23)) << 16 |
                        (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.w) * 255.0f, 23)) << 24;
  GfxModelCreateSoundObject(&self->base, 3);
  ActorCreateCollider(self);
  self->pushed = 0;
  ActorLoadMotions(self);
  CoreObjectListAppend(&self->base.base, g_actorList);
  self->routeStep = 0;
  ActorSetStateBase(self, 0, 0);
  self->detected = 0;
  self->alerted = 0;
  self->cleared357 = 0;
  self->route = NULL;
  self->routeTarget[0] = 0.0f;
  self->routeTarget[1] = 0.0f;
  self->routeTarget[2] = 0.0f;
  self->routeTarget[3] = 0.0f;
  self->checkpoint[0] = 0.0f;
  self->checkpoint[1] = 0.0f;
  self->checkpoint[2] = 0.0f;
  self->checkpoint[3] = 0.0f;
  self->stuckFrames = 0;
  self->patrolHold = 0;
  self->walkMotion = 1;
  self->routeWalkFirst = 1;
  return self;
}
