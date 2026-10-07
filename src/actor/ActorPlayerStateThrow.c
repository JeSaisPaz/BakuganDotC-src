// bdc 0x088e4c40 ActorPlayerStateThrow
#include "bdc.h"

/* State 7 (Bakugan throw) handler of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550
   bytes, constructor `ActorPlayerCtor`, vtable `0x08af38e4`) (vtable slot 27). Without a field
   camera `+0x314`, or when the previous state was 8, it goes straight back to state 0. Otherwise it
   damps the XZ velocity by 0.8, clears `throwLocked` and runs a step machine on `waitTimer`:
   0 = set up (aim point zeroed (VFPU bank C720 = 0), `"ret_aim"` billboard `aimSprite`, ball respawned
   when the team Bakugan changed, ball parked in the `Bip01_R_Hand`/`Bip01_L_Hand` node, aim camera
   `GameFieldCameraBeginAimView`, motion speed 2.0); 1 = wind-up (motion 0x1a at 0.8, aiming from
   0.6); 2 = aiming (`ActorPlayerUpdateThrowAim`): button 2 commits the throw (random voice
   `0x5200000 + rand(4)`, step 10), `0x400000` goes to step 100, `0x300001` drops the lock-on;
   10/11 = throw motion 0x19 with sound `0x2c00000`, reticle destroyed, ball released at 0.15
   (`ActorPlayerLaunchBall`); 12/13 = ball flight (`ActorPlayerUpdateBall`) with a 60-frame
   timeout; any other step ends the throw (trail reset when the timeout ran out, blend back to the
   follow camera, state 0). */

void ActorPlayerStateThrow(ActorPlayer *self)
{
  Actor *actor = &self->base;
  GfxSprite *aim;
  ActorBall *ball;
  SndManager *snd;
  u32 voice;
  u8 seen;
  s32 modelId;
  s32 timer;
  const VtblEntry *entry;
  float start[4];
  s32 i;

  if (actor->camera == NULL) {
    ActorSetState(actor, 0, 0);
    return;
  }
  if (actor->state == 8) {
    if (self->aimSprite != NULL) {
      aim = (GfxSprite *)self->aimSprite;
      aim->flags &= ~1u;
    }
    ((ActorBall *)self->ball)->noGroundProbe = 0;
    actor->noGroundProbe = 0;
    actor->waitTimer = 0;
    ActorSetState(actor, 0, 0);
    return;
  }
  /* velocity.xz *= 0.8 (vscl.t on xyz, only x and z stored back) */
  actor->base.velocity[0] = actor->base.velocity[0] * 0.8f;
  actor->base.velocity[2] = actor->base.velocity[2] * 0.8f;
  self->throwLocked = 0;

  switch (actor->waitTimer) {
  case 0:
    /* start = pos */
    for (i = 0; i < 4; i++) {
      start[i] = actor->base.pos[i];
    }
    SaveGetProfile();
    modelId = SaveGetTeamBakuganModelId(-1);
    if (self->ball != NULL && ((ActorBall *)self->ball)->inFlight != 0) {
      ActorSetState(actor, 0, 0);
      return;
    }
    /* aim point = bank constant C720 = (0, 0, 0, 0) */
    for (i = 0; i < 4; i++) {
      actor->aimPoint[i] = 0.0f;
    }
    if (GameFieldTaskExists() != 0 && self->aimSprite == NULL) {
      aim = GfxSpriteLayerCreateBillboardByName(((GameFieldTask *)GameFieldFindTask())->layers[1],
                                                "ret_aim");
      self->aimSprite = aim;
      /* aim->posX..posW = aimPoint */
      aim->posX = actor->aimPoint[0];
      aim->posY = actor->aimPoint[1];
      aim->posZ = actor->aimPoint[2];
      aim->posW = actor->aimPoint[3];
      aim->flags = aim->flags & ~1u;
      aim->flags = aim->flags | 0x20;
      aim->width = 6.0f;
      aim->height = 6.0f;
      aim->depth = 6.0f;
      aim->maybe_sizeW = 0.0f;
    }
    if (self->ballKind != modelId) {
      ActorPlayerSpawnBall(self, 0x58, NULL);
    }
    ActorBallSetIdle((CoreObject *)self->ball);
    ((ActorBall *)self->ball)->base.ambient[3] = 1.0f;
    ball = (ActorBall *)self->ball;
    ball->base.scale[3] = 0.0f;
    ball->base.scale[0] = 0.2f;
    ball->base.scale[1] = 0.2f;
    ball->base.scale[2] = 0.2f;
    if (self->rightHand != 0) {
      self->handNode = GfxModelFindNodeRecord(&actor->base, "Bip01_R_Hand");
    } else {
      self->handNode = GfxModelFindNodeRecord(&actor->base, "Bip01_L_Hand");
    }
    ball = (ActorBall *)self->ball;
    /* ball pos = start */
    for (i = 0; i < 4; i++) {
      ball->base.pos[i] = start[i];
    }
    ActorBallSetHeading(start[3], &((ActorBall *)self->ball)->base);
    if (self->handNode != NULL) {
      ((ActorBall *)self->ball)->flag1d1 = 1;
    }
    self->aimState[0] = 1;
    self->lockTarget = NULL;
    self->throwCommitted = 0;
    GameFieldCameraBeginAimView(actor->camera);
    actor->waitTimer = actor->waitTimer + 1;
    /* GfxModelSetMotionSpeed (vtable entry 6) */
    entry = &((const VtblEntry *)actor->base.base.vtable)[6];
    ((float (*)(void *, float))entry->fn)((char *)self + entry->delta, 2.0f);
    ((ActorBall *)self->ball)->noGroundProbe = 1;
    actor->noGroundProbe = 1;
    /* fall through */
  case 1:
    if (actor->base.motionEnded != 0 || GfxModelMotionReached(&actor->base, 0.8f)) {
      ActorPlayMotion(0.2f, self, 0x1a, 1, 0);
      entry = &((const VtblEntry *)actor->base.base.vtable)[6];
      ((float (*)(void *, float))entry->fn)((char *)self + entry->delta, 1.0f);
      actor->waitTimer = 2;
    }
    if (actor->base.motionEnded != 0 || GfxModelMotionReached(&actor->base, 0.6f)) {
      ActorPlayerUpdateThrowAim(self);
    }
    if ((actor->motion & 2) != 0 && GfxModelMotionReached(&actor->base, 0.8f)) {
      if (SndHasManager()) {
        snd = SndGetManager();
        voice = CoreRandNext(4);
        SndManagerPlay(snd, voice + 0x5200000, 0, 0);
      }
      self->throwCommitted = 1;
      actor->waitTimer = 10;
      return;
    }
    if ((actor->motion & 0x400000) == 0 && (actor->motion & 0x300001) != 0) {
      self->lockTarget = NULL;
      if (self->aimSprite != NULL) {
        aim = (GfxSprite *)self->aimSprite;
        aim->texture = GfxFindTexture("ret_aim");
        ((GfxSprite *)self->aimSprite)->flags |= 1;
      }
    }
    return;
  case 2:
    ActorPlayerUpdateThrowAim(self);
    if ((actor->motion & 2) != 0) {
      if (SndHasManager()) {
        snd = SndGetManager();
        voice = CoreRandNext(4);
        SndManagerPlay(snd, voice + 0x5200000, 0, 0);
      }
      self->throwCommitted = 1;
      actor->waitTimer = 10;
    } else if ((actor->motion & 0x400000) != 0) {
      actor->waitTimer = 100;
    } else if ((actor->motion & 0x300001) != 0) {
      self->lockTarget = NULL;
      if (self->aimSprite != NULL) {
        aim = (GfxSprite *)self->aimSprite;
        aim->texture = GfxFindTexture("ret_aim");
        ((GfxSprite *)self->aimSprite)->flags |= 1;
      }
    } else if (ActorFindPlayer() != NULL) {
      seen = self->aimScanSeen;
      if (seen != ((ActorPlayer *)ActorFindPlayer())->scan) {
        self->lockTarget = NULL;
        self->aimScanSeen = ((ActorPlayer *)ActorFindPlayer())->scan;
      }
    }
    self->throwLocked = 1;
    return;
  case 10:
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c00000, 0, 0);
    }
    entry = &((const VtblEntry *)actor->base.base.vtable)[6];
    ((float (*)(void *, float))entry->fn)((char *)self + entry->delta, 1.0f);
    ActorPlayMotion(0.2f, self, 0x19, 0, 0);
    self->ballFrames = 0;
    self->throwCommitted = 0;
    if (self->aimSprite != NULL) {
      if (self->aimSprite != NULL) {
        /* deleting destructor (vtable entry 1), flag 3 */
        aim = (GfxSprite *)self->aimSprite;
        entry = &((const VtblEntry *)aim->vtable)[1];
        ((void (*)(void *, int))entry->fn)((char *)aim + entry->delta, 3);
      }
      self->aimSprite = NULL;
    }
    actor->camera->stepState = 2;
    actor->waitTimer = actor->waitTimer + 1;
    /* fall through */
  case 11:
    if (GfxModelMotionReached(&actor->base, 0.15f)) {
      ActorPlayerLaunchBall(self);
      self->handNode = NULL;
      actor->waitTimer = actor->waitTimer + 1;
    }
    return;
  case 12:
    if (!GfxModelMotionReached(&actor->base, 0.8f)) {
      if (ActorPlayerUpdateBall(self) == 0) {
        return;
      }
    }
    actor->stateTimer = 60;
    actor->waitTimer = actor->waitTimer + 1;
    /* fall through */
  case 13:
    if (ActorPlayerUpdateBall(self) == 0) {
      timer = actor->stateTimer;
      actor->stateTimer = timer - 1;
      if (timer > 0) {
        return;
      }
    }
    actor->waitTimer = actor->waitTimer + 1;
    /* fall through */
  default:
    break;
  }

  /* end of the throw (steps 3..9, 14 and up, 100) */
  if (actor->stateTimer <= 0) {
    self->throwing = 0;
    ActorBallTrailReset((ActorBallTrail *)self->trail, false);
    ((ActorBall *)self->ball)->base.ambient[3] = 0.0f;
  }
  if (self->aimSprite != NULL) {
    aim = (GfxSprite *)self->aimSprite;
    aim->flags &= ~1u;
  }
  self->aimState[0] = 0;
  self->lockTarget = NULL;
  GameFieldCameraBeginBlendToFollow(actor->camera);
  ((ActorBall *)self->ball)->noGroundProbe = 0;
  actor->noGroundProbe = 0;
  ActorSetState(actor, 0, 0);
}
