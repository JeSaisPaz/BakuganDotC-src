// bdc 0x088b1230 ActorStageObjPropState02Knocked
#include "bdc.h"

/* State 2 of the knock-over prop (sub-step `step`). Step 0 creates the 0x170-byte physics box
   `physicsBox` once (low-heap `MemAlloc`, `CollisionPhysBoxCtor`), sets it up from the bounds
   and model matrix (`CollisionPhysBoxInit`, centred, no ground raycast, gravity 2, centre = model
   position), plays the hit sound (word 0 of `soundParams`), snaps `floorY` to the ground under the
   prop when it differs by more than 20 (`CollisionRaycastPoint` from 9999 above) and launches the
   box (`CollisionPhysBoxApplyImpulse`) along the direction away from the hitter (model position -
   `contactPos`): for `ActorStageObjKindTopples` kinds 0.05 * dir + 0.3 * `contactVel` with y = 10,
   otherwise 0.3 * dir + 0.15 * `contactVel` with y clamped to >= 0 plus 10.
   Steps 1..3 first test `ActorStageObjPropCheckLanded` (landed in water -> step 10) and step the
   box (vtable slot 2, `CollisionPhysBoxStep`, returns the motion). Step 1: motion < 0.1 -> next
   step; otherwise on the box's first bounce (`stepCounter` != 0) plays sound word 2 and spawns
   effect 0x1d at the top of the bounds, then next step. Step 2: for kinds 0x13, 0x1e, 0x1f, 0x38,
   0x39, 0x3c, 0x58, 0x59, 0x69, 0x7b, 0x84, 0x8a, 0xa8, 0xb2 counts `settleFrames` and, once the
   motion is < 0.1 or 15 frames passed, makes the materials translucent
   (`ActorStageObjPropKnockedMaterialSetTranslucent`), spawns effect 7 at the prop and sets `fade`
   0.5; other kinds only make the materials translucent once the motion is < 0.1; then next step.
   Step 3 fades out by 0.08 per frame; at 0 clears the ambient alpha, updates the lights
   (`ActorStageObjUpdateLightAlpha`) and calls the virtual break (vtable entry 11), stepping the
   box once more first for kinds 0x13, 0x58, 0x59, 0xa8. Step 10 (water) steps the box, spawns
   effects 0xb (top of the bounds) and 0xc (at the water bed height,
   `BtlStageGetWaterBedHeight`), plays sound 0x20000f and clears `settleFrames`; step 11 steps the
   box and calls the virtual break once the motion is < 0.1 or after 15 frames. Every call ends by
   copying the model's root matrix translation into the model position.
   The normalisation uses 0 for a zero-length direction and leaves the impulse's w at 0 (bank S713). */

static float ActorStageObjPropState02KnockedStepBox(CollisionPhysBox *box)
{
  const VtblEntry *e = &box->vtbl[2];
  return ((float (*)(void *))e->fn)((u8 *)box + e->delta);
}

static void ActorStageObjPropState02KnockedBreak(ActorStageObjProp *self)
{
  const VtblEntry *brk = &((const VtblEntry *)self->base.base.base.vtable)[11];
  ((void (*)(void *))brk->fn)((u8 *)self + brk->delta);
}

void ActorStageObjPropState02Knocked(ActorStageObjProp *self)
{
  float dir[4];
  float vel[3];
  float spawn[4];
  float water[4];
  bool topples;
  float dirScale;
  float velScale;
  float len2;
  float inv;
  float k;
  float *pos;
  CollisionPhysBox *box;
  CollisionPhysBox *mem;
  float *mtx;
  float *bounds;
  bool fromLow;
  float y;
  float fade;

  pos = self->base.base.pos;
  switch (self->step) {
  case 0:
    if (self->physicsBox == NULL) {
      box = NULL;
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = (CollisionPhysBox *)MemAlloc(sizeof(CollisionPhysBox), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (mem != NULL) {
        CollisionPhysBoxCtor(mem);
        box = mem;
      }
      self->physicsBox = box;
    }
    box = (CollisionPhysBox *)self->physicsBox;
    mtx = self->base.base.data->rootMatrix;
    bounds = ActorStageObjGetBounds(&self->base);
    CollisionPhysBoxInit(box, mtx, (const ScePspFVector4 *)bounds, true);
    ((CollisionPhysBox *)self->physicsBox)->raycastGround = 0;
    ((CollisionPhysBox *)self->physicsBox)->groundFlag = 0;
    ((CollisionPhysBox *)self->physicsBox)->gravity = 2.0f;
    mtx = &self->base.base.data->rootMatrix[12];
    box = (CollisionPhysBox *)self->physicsBox;
    box->centre.x = mtx[0];
    box->centre.y = mtx[1];
    box->centre.z = mtx[2];
    box->centre.w = mtx[3];
    ActorStageObjPropPlaySound(self, ((s32 *)self->base.soundParams)[0]);
    mtx = &self->base.base.data->rootMatrix[12];
    dir[0] = mtx[0];
    dir[1] = mtx[1];
    dir[2] = mtx[2];
    dir[3] = mtx[3];
    dir[1] = dir[1] + 9999.0f;
    if (CollisionRaycastPoint(dir, dir) != 0) {
      box = (CollisionPhysBox *)self->physicsBox;
      if (!(fabsf(box->floorY - dir[1]) <= 20.0f)) {
        ((CollisionPhysBox *)self->physicsBox)->floorY = dir[1];
      }
    }
    /* dir = model position - contactPos (w keeps the model's w) */
    mtx = &self->base.base.data->rootMatrix[12];
    dir[0] = mtx[0] - self->contactPos[0];
    dir[1] = mtx[1] - self->contactPos[1];
    dir[2] = mtx[2] - self->contactPos[2];
    dir[3] = mtx[3];
    topples = ActorStageObjKindTopples(&self->base) != 0;
    if (topples) {
      dirScale = 0.05f;
      velScale = 0.3f;
    } else {
      dirScale = 0.3f;
      velScale = 0.15f;
    }
    /* dir = normalise(dir) * dirScale + contactVel * velScale; w = 0 (bank S713) */
    len2 = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    inv = VfRsq(len2);
    if (len2 == 0.0f) {
      inv = 0.0f;
    }
    k = inv * dirScale;
    dir[0] = dir[0] * k;
    dir[1] = dir[1] * k;
    dir[2] = dir[2] * k;
    dir[3] = 0.0f;
    vel[0] = self->contactVel[0] * velScale;
    vel[1] = self->contactVel[1] * velScale;
    vel[2] = self->contactVel[2] * velScale;
    dir[0] = dir[0] + vel[0];
    dir[1] = dir[1] + vel[1];
    dir[2] = dir[2] + vel[2];
    if (topples) {
      dir[1] = 10.0f;
    } else {
      if (dir[1] < 0.0f) {
        dir[1] = 0.0f;
      }
      dir[1] = dir[1] + 10.0f;
    }
    CollisionPhysBoxApplyImpulse(1.0f, (CollisionPhysBox *)self->physicsBox, dir,
                                 (const ScePspFVector4 *)self->contactPos);
    ((CollisionPhysBox *)self->physicsBox)->stepCounter = 0;
    self->settleFrames = 0;
    self->step = self->step + 1;
    break;
  case 1:
    if (ActorStageObjPropCheckLanded(self) != 0) {
      self->step = 10;
      break;
    }
    if (ActorStageObjPropState02KnockedStepBox((CollisionPhysBox *)self->physicsBox) < 0.1f) {
      self->step = self->step + 1;
      break;
    }
    if (((CollisionPhysBox *)self->physicsBox)->stepCounter == 0) {
      break;
    }
    ActorStageObjPropPlaySound(self, ((s32 *)self->base.soundParams)[2]);
    spawn[0] = pos[0];
    spawn[1] = pos[1];
    spawn[2] = pos[2];
    spawn[3] = pos[3];
    y = spawn[1];
    bounds = ActorStageObjGetBounds(&self->base);
    spawn[1] = y + bounds[1];
    GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x1d, spawn, self);
    self->step = self->step + 1;
    break;
  case 2:
    if (ActorStageObjPropCheckLanded(self) != 0) {
      self->step = 10;
      break;
    }
    switch (self->base.kind) {
    case 0x13:
    case 0x1e:
    case 0x1f:
    case 0x38:
    case 0x39:
    case 0x3c:
    case 0x58:
    case 0x59:
    case 0x69:
    case 0x7b:
    case 0x84:
    case 0x8a:
    case 0xa8:
    case 0xb2:
      self->settleFrames = self->settleFrames + 1;
      if (ActorStageObjPropState02KnockedStepBox((CollisionPhysBox *)self->physicsBox) < 0.1f ||
          self->settleFrames == 15) {
        GfxModelForEachMaterial(&self->base.base, ActorStageObjPropKnockedMaterialSetTranslucent,
                                NULL);
        GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 7, pos, self);
        self->base.fade = 0.5f;
        self->step = self->step + 1;
      }
      break;
    default:
      if (ActorStageObjPropState02KnockedStepBox((CollisionPhysBox *)self->physicsBox) < 0.1f) {
        GfxModelForEachMaterial(&self->base.base, ActorStageObjPropKnockedMaterialSetTranslucent,
                                NULL);
        self->step = self->step + 1;
      }
      break;
    }
    break;
  case 3:
    if (ActorStageObjPropCheckLanded(self) != 0) {
      self->step = 10;
      break;
    }
    fade = self->base.fade - 0.08f;
    self->base.fade = fade;
    if (!(fade <= 0.0f)) {
      break;
    }
    self->base.fade = 0.0f;
    self->base.base.ambient[3] = 0.0f;
    ActorStageObjUpdateLightAlpha(&self->base);
    switch (self->base.kind) {
    case 0x13:
    case 0x58:
    case 0x59:
    case 0xa8:
      ActorStageObjPropState02KnockedStepBox((CollisionPhysBox *)self->physicsBox);
      break;
    default:
      break;
    }
    ActorStageObjPropState02KnockedBreak(self);
    break;
  case 10:
    ActorStageObjPropState02KnockedStepBox((CollisionPhysBox *)self->physicsBox);
    spawn[0] = pos[0];
    spawn[1] = pos[1];
    spawn[2] = pos[2];
    spawn[3] = pos[3];
    y = spawn[1];
    bounds = ActorStageObjGetBounds(&self->base);
    spawn[1] = y + bounds[1];
    GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0xb, spawn, self);
    water[0] = pos[0];
    water[1] = pos[1];
    water[2] = pos[2];
    water[3] = pos[3];
    water[1] = BtlStageGetWaterBedHeight();
    GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0xc, water, self);
    ActorStageObjPropPlaySound(self, 0x20000f);
    self->settleFrames = 0;
    self->step = self->step + 1;
    break;
  case 11:
    if (ActorStageObjPropState02KnockedStepBox((CollisionPhysBox *)self->physicsBox) < 0.1f) {
      ActorStageObjPropState02KnockedBreak(self);
      self->step = self->step + 1;
    } else if (self->settleFrames < 15) {
      self->settleFrames = self->settleFrames + 1;
    } else {
      ActorStageObjPropState02KnockedBreak(self);
      self->step = self->step + 1;
    }
    break;
  default:
    break;
  }
  mtx = &self->base.base.data->rootMatrix[12];
  pos[0] = mtx[0];
  pos[1] = mtx[1];
  pos[2] = mtx[2];
  pos[3] = mtx[3];
}
