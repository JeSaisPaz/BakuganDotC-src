// bdc 0x088e624c ActorNpcCanSeePlayer
#include "bdc.h"

/* Vision test of the field NPC/guard actor classes (models 0x4e..0x53 `npc_sm_we/mi/st`,
   `npc_sr_we/mi/st`; base constructor `ActorNpcCtor`, vtables `0x08af3b74`, `0x08af39e4`,
   `0x08af3d04`, `0x08af3e94`) against the player (`ActorFindPlayer`). Returns 0 without
   touching the NPC during field sub-states 5 and 0x1e (`GameFieldTask::subState`). Otherwise
   the player counts as absent when save-profile word 0x2f is positive, and as invisible while
   its power A is active (`ActorPlayerIsStealthed`) or it is in state 10. A visible player is
   tested for distance (`viewDist`), view cone (`viewHalfAngle` around the heading stored in
   `viewHeading`) and a collision line of sight (`CollisionRaycastRayB`) plus
   `ActorNpcIsPlayerHeightApart`; the result (2 = spotted, else 0) is stored in `seeResult`
   and returned. A touched (`pushed`) `npc_sm_*` NPC that did not spot the player, in an AI
   state other than 4..7 / 11, records the player position (`noisePoint`) and its own
   (`returnPoint`) and switches to AI state 7. `pushed` is cleared on every non-early path. */

s32 ActorNpcCanSeePlayer(ActorNpc *self)
{
  GameFieldTask *field;
  ActorPlayer *player;
  s32 result;
  s32 noChase;
  s32 hidden;
  float d[3];
  float dist;
  float toPlayer;
  float heading;
  float diff;
  float playerPos[4];
  float origin[4];
  float dir[4];
  s32 i;

  field = (GameFieldTask *)GameFieldFindTask();
  if (field->subState == 5 || field->subState == 0x1e) {
    return 0;
  }

  result = 0;
  noChase = 0;
  if ((s32)self->base.base.base.unk08 > 0x50 && (s32)self->base.base.base.unk08 < 0x54) {
    noChase = 1;
  }

  player = (ActorPlayer *)ActorFindPlayer();
  if (SaveHasProfile() && (s32)SaveProfileGetWord(SaveGetProfile(), 0x2f) > 0) {
    player = NULL;
  }
  if (player == NULL) {
    self->base.pushed = 0;
    self->seeResult = result;
    return result;
  }

  hidden = 0;
  if (player->base.state == 10) {
    hidden = 1;
  }
  if (self->base.pushed) {
    if (ActorPlayerIsStealthed(player)) {
      ActorPlayerStealthOff(player, 1);
      if (player->base.state != 0xd && !self->revealedPlayer) {
        ActorSetState(&player->base, 0xd, 0);
        self->revealedPlayer = 1;
      }
    }
  } else {
    if (ActorPlayerIsStealthed(player)) {
      hidden = 1;
    }
    if (self->revealedPlayer) {
      self->revealedPlayer = 0;
    }
  }

  if (!hidden) {
    /* |self - player| (xyz) */
    d[0] = self->base.base.pos[0] - player->base.base.pos[0];
    d[1] = self->base.base.pos[1] - player->base.base.pos[1];
    d[2] = self->base.base.pos[2] - player->base.base.pos[2];
    dist = __builtin_sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    if (dist < self->viewDist) {
      /* stack copy of the player position */
      for (i = 0; i < 4; i++) {
        playerPos[i] = player->base.base.pos[i];
      }
      toPlayer = atan2f(playerPos[2] - self->base.base.pos[2],
                        playerPos[0] - self->base.base.pos[0]);
      heading = ActorNpcGetViewHeading(self);
      self->viewHeading = heading;
      diff = (toPlayer + 12.566371f) - (heading + 6.2831855f);
      if (!(diff <= 3.1415927f)) {
        diff = diff - 6.2831855f;
      } else if (diff <= -3.1415927f) {
        diff = diff + 6.2831855f;
      }
      if (!(diff <= 3.1415927f)) {
        diff = diff - 6.2831855f;
      }
      if (diff < -3.1415927f) {
        diff = diff + 6.2831855f;
      }
      if (diff < 0.0f) {
        diff = -diff;
      }
      if (diff < self->viewHalfAngle) {
        /* origin = self pos; dir = player - self (xyz), w = player w (vsub.t) */
        for (i = 0; i < 4; i++) {
          origin[i] = self->base.base.pos[i];
        }
        dir[0] = player->base.base.pos[0] - self->base.base.pos[0];
        dir[1] = player->base.base.pos[1] - self->base.base.pos[1];
        dir[2] = player->base.base.pos[2] - self->base.base.pos[2];
        dir[3] = player->base.base.pos[3];
        if (g_scriptGlobalVars[1] == 0xc || g_scriptGlobalVars[1] == 0xe) {
          origin[1] = origin[1] + 14.5f;
        } else {
          origin[1] = origin[1] + 10.0f;
        }
        if (CollisionRaycastRayB(0x6f800700, origin, dir) == NULL &&
            ActorNpcIsPlayerHeightApart(self) == 0) {
          result = 2;
        }
      }
    }
  }

  if (!noChase && result != 2 && self->base.pushed) {
    switch (self->aiState) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 11:
      break;
    default: /* 8, 9, 10 and out of table range */
      for (i = 0; i < 4; i++) {
        self->noisePoint[i] = player->base.base.pos[i];
      }
      for (i = 0; i < 4; i++) {
        self->returnPoint[i] = self->base.base.pos[i];
      }
      self->aiState = 7;
      self->subStep = 0;
      break;
    }
  }
  self->base.pushed = 0;
  self->seeResult = result;
  return result;
}
