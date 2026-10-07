// bdc 0x088e23d8 ActorPlayerUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 7) of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550
   bytes, constructor `ActorPlayerCtor`, vtable `0x08af38e4`): debug face-expression cycling on the
   pad, reads the controller command flags (`BtlInputReadActions(input)` -> `motion`, with the
   one-shot `motionOverride`), runs the behaviour slot (vtable entry 31, the state machine), keeps
   the idle-fidget timer (`idleHint`/`idleFrames`, 150 frames in states 0/6 while not scanning),
   updates the two gauntlet powers (`ActorPlayerUpdatePowers`, when `powersEnabled`), then the
   movement (`ActorApplyGravity`, skipped in state 8), animation
   (`GfxModelUpdateMotion`/`GfxModelApplyMotion`), vtable entry 10, the ball trail, colliders,
   ground probe, the shadow (billboard moved by `modelOffset`), footsteps and finally eases
   `tiltQuat` 20% toward `g_vecUp` and renormalises its xyz (clamped to [-1, 1]; scale 0 when the
   xyz has zero length), writing 0 to `tiltQuat[3]` (the bank zero S713 in the masked w lane). */

void ActorPlayerUpdate(ActorPlayer *self)
{
  Actor *actor;
  const VtblEntry *entry;
  s32 face;
  s32 motion;
  s32 override;
  s32 idle;
  s32 state;
  ActorBall *ball;
  GfxSprite *billboard;
  float *tilt;
  float lenSq;
  float k;
  float x;
  float y;
  float z;

  if ((g_padState->buttons & 0x1000) != 0 && (g_padState->pressed & 0x10) != 0) {
    face = self->base.faceExpression + 1;
    self->base.faceExpression = face;
    if (!(face < 5)) {
      self->base.faceExpression = 0;
    }
    for (actor = *(Actor **)ActorGetList(); actor != NULL; actor = (Actor *)actor->base.base.next) {
      if (actor->base.base.unk08 == 0x23) {
        ActorSetFaceExpression(actor, self->base.faceExpression & 0xff);
        break;
      }
    }
  }

  self->base.motion = BtlInputReadActions(self->base.input);
  if (self->base.motionOverride != 0) {
    motion = 0;
    override = self->base.motionOverride - 1;
    self->base.motionOverride = override;
    if (override == 0) {
      motion = self->base.motionOverrideTimer;
    }
    self->base.motion = motion;
  }
  self->base.flags = self->base.flags & 0xce1ffff8;
  self->cleared3ac = 0;
  entry = (const VtblEntry *)self->base.base.base.vtable + 31;
  self->talkTargetInReach = 0;
  self->triggerInReach = 0;
  ((void (*)(void *))entry->fn)((char *)self + entry->delta);

  if (self->promptA == 0 && self->idleHint == 0) {
    if (self->base.state == 0 || self->base.state == 6) {
      if (self->scan == 0) {
        idle = self->idleFrames + 1;
        self->idleFrames = idle;
        if (!(idle < 0x97)) {
          self->idleHint = 1;
          self->idleFrames = 0;
        }
      } else {
        self->idleFrames = 0;
      }
    } else {
      self->idleFrames = 0;
    }
  } else {
    state = self->base.state;
    self->idleFrames = 0;
    if (state == 0 || self->base.state == 6) {
      if (self->scan != 0) {
        self->idleHint = 0;
      }
    } else {
      self->idleHint = 0;
    }
    if (self->base.state == 2 || self->base.state == 3 || self->base.state == 9) {
      self->promptA = 0;
    }
  }

  if (self->powersEnabled != 0) {
    ActorPlayerUpdatePowers(self);
  }
  if (self->base.state != 8) {
    self->base.flags = self->base.flags & 0x7ffdfdef;
    ActorApplyGravity(&self->base);
  }
  GfxModelUpdateMotion(&self->base.base);
  GfxModelApplyMotion(&self->base.base);
  entry = (const VtblEntry *)self->base.base.base.vtable + 10;
  ((void (*)(void *))entry->fn)((char *)self + entry->delta);

  if (self->trail != NULL) {
    ((ActorBallTrail *)self->trail)->enable = self->throwing;
    ActorBallTrailUpdate(self->trail);
    if (self->throwing != 0 && self->ball != NULL && ((ActorBall *)self->ball)->flag1d3 != 0) {
      self->throwing = 0;
      ActorBallTrailReset(self->trail, true);
      ball = (ActorBall *)self->ball;
      ball->base.ambient[3] = 0.0f;
    }
  }
  ActorSyncColliders(&self->base);
  ActorProbeGround(&self->base);
  self->base.noGroundProbe = 0;
  BtlShadowUpdate(self->base.shadow);

  /* shadow billboard position xyz += modelOffset xyz (w kept) */
  billboard = ((BtlShadow *)self->base.shadow)->billboard;
  billboard->posX = billboard->posX + self->modelOffset[0];
  billboard->posY = billboard->posY + self->modelOffset[1];
  billboard->posZ = billboard->posZ + self->modelOffset[2];
  ActorUpdateFootsteps(&self->base);

  if ((self->base.flags & 1) == 0) {
    /* tiltQuat += (g_vecUp - tiltQuat) * 0.2; then xyz normalised and clamped to [-1, 1]
       (0 if zero length), w = 0 */
    tilt = self->base.tiltQuat;
    tilt[0] = tilt[0] + (g_vecUp.x - tilt[0]) * 0.2f;
    tilt[1] = tilt[1] + (g_vecUp.y - tilt[1]) * 0.2f;
    tilt[2] = tilt[2] + (g_vecUp.z - tilt[2]) * 0.2f;
    tilt[3] = tilt[3] + (g_vecUp.w - tilt[3]) * 0.2f;
    lenSq = tilt[0] * tilt[0] + tilt[1] * tilt[1] + tilt[2] * tilt[2];
    k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    x = VfSat1(tilt[0] * k);
    y = VfSat1(tilt[1] * k);
    z = VfSat1(tilt[2] * k);
    tilt[0] = x;
    tilt[1] = y;
    tilt[2] = z;
    tilt[3] = 0.0f;
  }
}
