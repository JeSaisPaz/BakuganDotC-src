// bdc 0x088e0d54 ActorPlayerCtor
#include "bdc.h"

/* Constructor of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550 bytes, vtable
   `g_actorPlayerVtbl`): `ActorCtor`, stops the power effects 0x26/0x44/10 left on its anchor
   (the actor position row `mtx[12]`) in `g_worldEffectMgr`, clears the power state (stealth,
   scan, gauges = 1.0), copies the profile's point counter (`SaveGetProfile`) to `points`, resets
   the sound handles, the held-object and aim fields, sets an identity matrix in `mtx` (VFPU
   `vmidt.q`), zeroes the aim vectors, the lock point and the model offset (bank constant C720 = 0),
   creates the throw trail (`ActorPlayerCreateBallTrail`) when the field task exists
   (`GameFieldTaskExists`) and zeroes `focusPoint`. Returns `self`. */

ActorPlayer *ActorPlayerCtor(ActorPlayer *self, s32 modelId)
{
  float *attach;
  s32 i;

  ActorCtor(&self->base, modelId);
  self->base.base.base.vtable = (void *)&g_actorPlayerVtbl;
  self->aimScanSeen = 0;
  self->behaviourMark = 0xff;
  self->stealth = 0;
  attach = &self->base.mtx[12];
  if (g_worldEffectMgr != NULL) {
    GfxEffectStopAttached(g_worldEffectMgr, 0x26, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 0x44, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 10, attach);
  }
  self->scan = 0;
  self->powerCooldown = 0;
  self->cleared3ac = 0;
  self->talkTargetInReach = 0;
  self->triggerInReach = 0;
  self->throwLocked = 0;
  self->idleHint = 0;
  self->promptA = 0;
  self->idleFrames = 0;
  self->gaugeA = 1.0f;
  self->gaugeB = 1.0f;
  self->holdEffects[0] = NULL;
  self->holdEffects[1] = NULL;
  self->holdEffects[2] = NULL;
  self->powersEnabled = 0;
  self->talkTarget = NULL;
  self->points = SaveGetProfile()->data->points;
  self->penaltySound = 0xffffffff;
  self->inChargeZone = 0;
  self->loopSound = 0;
  self->loopSoundPlaying = 0;
  self->scanSound = 0;
  self->ball = NULL;
  self->handNode = NULL;
  for (i = 0; i < 16; i++) {
    self->mtx[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }
  self->trail = NULL;
  self->throwing = 0;
  self->rightHand = 1;
  self->aimState[0] = 0;
  for (i = 0; i < 4; i++) {
    self->aimHit[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    self->aimOrigin[i] = 0.0f;
  }
  self->aimSprite = NULL;
  self->aimState[1] = 0;
  self->lockTarget = NULL;
  for (i = 0; i < 4; i++) {
    self->lockPoint[i] = 0.0f;
  }
  self->ballKind = -1;
  self->base.placement = NULL;
  self->base.cleared344 = 0;
  for (i = 0; i < 4; i++) {
    self->modelOffset[i] = 0.0f;
  }
  if (GameFieldTaskExists() != 0) {
    ActorPlayerCreateBallTrail(self);
  }
  self->caughtBy = NULL;
  self->ballFrames = 0;
  self->ballTarget = NULL;
  for (i = 0; i < 4; i++) {
    self->focusPoint[i] = 0.0f;
  }
  return self;
}
