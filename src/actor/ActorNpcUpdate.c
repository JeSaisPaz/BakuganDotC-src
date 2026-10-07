// bdc 0x088e6e10 ActorNpcUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 34, `+0x114`) of the field NPC/guard actor classes (models
   0x4e..0x53 `npc_sm_we/mi/st`, `npc_sr_we/mi/st`; base constructor `ActorNpcCtor`): copies the body
   heading into `viewHeading`, picks the view distance (300, 450 while the active field camera is in
   mode 7), runs slot 32 (`+0x100`), masks `flags`, recomputes `viewHeading`
   (`ActorNpcGetViewHeading`) and runs the behaviour slot 31 (`+0xf8`). Unless the NPC is in state 4,
   6 or 8 or `forceUpdate` is set, when `ActorNpcCanSeePlayer` returns 2, no catch is active
   (`catchActive` of the field task) and the player is not `detected`, it starts the catch: saves the
   position as `returnPoint` (unless in state 5), sets `catchActive`, enters state 4 (sub step 0) and
   plays its voice (`ActorPlayVoice`). Then the distance cull slot 5 (2.4..view distance) sets
   `culled`. Not culled (or state 8): gravity when moving (`ActorApplyGravity`), motion
   (`GfxModelUpdateMotion`/`GfxModelApplyMotion`), slot 10, `ActorSyncColliders`, ground probe
   when moving and not on this NPC's frame slot (`ActorProbeGround`), `ActorUpdateFootsteps` and,
   unless `flags` bit 0, easing `tiltQuat` toward `g_vecUp`. Culled: gravity when moving, motion only
   in state 6, slot 10, and returns early when `placementSlot % 4` differs from the field frame
   counter `% 4`; otherwise colliders and (when moving) the ground probe. Both paths that do not
   return early end with `BtlShadowUpdate`, slot 47 (with the view distance) and slot 49. */

void ActorNpcUpdate(ActorNpc *self)
{
  const VtblEntry *vt;
  GameFieldCamera *cam;
  GameFieldTask *field;
  s32 moving;
  s32 slot;
  s32 state;
  float *tilt;
  float lenSq;
  float invLen;
  float speedSq;
  float viewDist;

  self->viewHeading = self->base.base.rot[1];
  cam = (GameFieldCamera *)g_gfxActiveCamera;
  viewDist = 300.0f;
  if (cam != NULL) {
    switch (cam->mode) {
    case 7:
      viewDist *= 1.5f;
      break;
    default:
      break;
    }
  }
  vt = &((const VtblEntry *)self->base.base.base.vtable)[32];
  ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
  self->base.flags &= 0x4e1dffe8;
  self->viewHeading = ActorNpcGetViewHeading(self);
  vt = &((const VtblEntry *)self->base.base.base.vtable)[31];
  ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);

  state = self->aiState;
  if (state != 8 && state != 6 && state != 4 && self->forceUpdate == 0 &&
      ActorNpcCanSeePlayer(self) == 2 &&
      ((GameFieldTask *)CoreTaskFind(500))->catchActive == 0 &&
      ((Actor *)ActorFindPlayer())->detected == 0) {
    if (self->aiState != 5) {
      self->returnPoint[0] = self->base.base.pos[0];
      self->returnPoint[1] = self->base.base.pos[1];
      self->returnPoint[2] = self->base.base.pos[2];
      self->returnPoint[3] = self->base.base.pos[3];
    }
    ((GameFieldTask *)CoreTaskFind(500))->catchActive = 1;
    self->aiState = 4;
    self->subStep = 0;
    ActorPlayVoice(self, self->voiceId);
  }

  self->culled = 0;
  vt = &((const VtblEntry *)self->base.base.base.vtable)[5];
  if (((s32 (*)(void *, float *, float *, float, float))vt->fn)(
          (u8 *)self + vt->delta, &self->base.base.data->rootMatrix[12],
          &self->base.base.ambient[3], 2.4f, viewDist) != 0) {
    self->culled = 1;
  }

  speedSq = self->base.base.velocity[0] * self->base.base.velocity[0] +
            self->base.base.velocity[1] * self->base.base.velocity[1] +
            self->base.base.velocity[2] * self->base.base.velocity[2];
  moving = 0;
  if (!(speedSq <= 1e-05f)) {
    moving = 1;
  }

  if (self->culled == 0 || self->aiState == 8) {
    if (moving != 0) {
      ActorApplyGravity(&self->base);
    }
    GfxModelUpdateMotion(&self->base.base);
    GfxModelApplyMotion(&self->base.base);
    vt = &((const VtblEntry *)self->base.base.base.vtable)[10];
    ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
    ActorSyncColliders(&self->base);
    if (moving != 0) {
      slot = (s32)self->base.placementSlot % 4;
      field = (GameFieldTask *)GameFieldFindTask();
      if (slot != (s32)field->frameCount % 4) {
        ActorProbeGround(&self->base);
        self->base.noGroundProbe = 0;
      }
    }
    ActorUpdateFootsteps(&self->base);
    if ((self->base.flags & 1) == 0) {
      /* tiltQuat += (g_vecUp - tiltQuat) * 0.2, then xyz normalised and clamped to [-1, 1]; w is
         the bank constant S713 (0), and a zero length scales by S713 (0) too */
      tilt = self->base.tiltQuat;
      tilt[0] = tilt[0] + (g_vecUp.x - tilt[0]) * 0.2f;
      tilt[1] = tilt[1] + (g_vecUp.y - tilt[1]) * 0.2f;
      tilt[2] = tilt[2] + (g_vecUp.z - tilt[2]) * 0.2f;
      tilt[3] = tilt[3] + (g_vecUp.w - tilt[3]) * 0.2f;
      lenSq = tilt[0] * tilt[0] + tilt[1] * tilt[1] + tilt[2] * tilt[2];
      invLen = VfRsq(lenSq);
      if (lenSq == 0.0f) {
        invLen = 0.0f;
      }
      tilt[0] = VfSat1(tilt[0] * invLen);
      tilt[1] = VfSat1(tilt[1] * invLen);
      tilt[2] = VfSat1(tilt[2] * invLen);
      tilt[3] = 0.0f;
    }
  } else {
    if (moving != 0) {
      ActorApplyGravity(&self->base);
    }
    if (self->aiState == 6) {
      GfxModelUpdateMotion(&self->base.base);
      GfxModelApplyMotion(&self->base.base);
    }
    vt = &((const VtblEntry *)self->base.base.base.vtable)[10];
    ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
    slot = (s32)self->base.placementSlot % 4;
    field = (GameFieldTask *)GameFieldFindTask();
    if (slot != (s32)field->frameCount % 4) {
      return;
    }
    ActorSyncColliders(&self->base);
    if (moving != 0) {
      ActorProbeGround(&self->base);
      self->base.noGroundProbe = 0;
    }
  }
  BtlShadowUpdate(self->base.shadow);
  vt = &((const VtblEntry *)self->base.base.base.vtable)[47];
  ((void (*)(void *, float))vt->fn)((u8 *)self + vt->delta, viewDist);
  vt = &((const VtblEntry *)self->base.base.base.vtable)[49];
  ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
}
