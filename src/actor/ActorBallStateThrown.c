// bdc 0x088b8cb4 ActorBallStateThrown
#include "bdc.h"

/* State 1 handler of the `ActorBall` (`ActorBallCtor`, entered by `ActorBallBeginThrow`), a
   step machine on `step`:
   - step 0 arms the flight (`travelled` = 0, `flag1d0` = 0, `inFlight` = 1) and goes to step 1;
   - step 1 sweeps from `pos` along `velocity` with radius 1.4 (`ActorBallCheckHit`, attack 0xbb).
     On a hit it calls `ActorNotifyNearestGuardOfNoise` at `vec1e0` (1 for hit type 1, 3 for hit
     type 3, else 0); if the map was hit (`flag1d0`) the direction is the normalised
     `pos - contact` of `g_actorBallHitQuery` (on a floor hit, `ActorBallHitIsFloor`, the contact
     is raised 0.2 and the direction becomes the hit normal moved 2% toward the camera direction of
     `g_gfxActiveCamera`), the contact is copied to `hitPoint`, sound `0x2c00022` is played there
     (`SndEmitterCreateAtPos`, if a listener exists) and effect 7 of `g_worldEffectMgr` is spawned
     there along the direction (`GfxEffectSpawnDirected`); then step 2. Without a hit it adds
     |velocity| to `travelled` and goes to step 2 once that is no longer <= 150.
   - step 2 sets up the return flight: random `wobbleYaw` in [-pi/10, pi/10), `wobblePitch` pi/10,
     `wobbleScale` 1, `homeSpeed` 8, 25 frames (`flag1fc`), `flag1d2` = 1; then step 10 in the same
     frame (9999 without an owner).
   - step 10 flies back toward the left upper arm of the owner (`ActorGetLeftUpperArmPos`): ends
     (step 11) when within 3 units or after the frame budget runs out; otherwise moves `pos` by the
     direction (yaw/pitch to the target plus the decaying wobble) times `homeSpeed` (+0.2 per frame
     up to 16, capped at 1.5*sqrt(distance)), plus the owner's velocity unless the owner's flags
     have any of 0x20210. Without an owner step 9999.
   - any other step (9999, 11) ends the throw: `flag1d3` = 1, `inFlight` = 0, state 0
     (`ActorBallSetState`).
   The VFPU bank constants are literals: 0 (zero-length normalise fallback and the stored w lane),
   2/pi (radians to quarter turns, folded into sinf/cosf) and 1 (`vrndf1` [1,2) to [0,1)). */

void ActorBallStateThrown(CoreObject *ball)
{
  ActorBall *b = (ActorBall *)ball;
  float dir[4];
  float armPos[4];
  float delta[4];
  float *pos;
  float *vel;
  float *cam;
  float lenSq;
  float k;
  float dist;
  float limit;
  float yaw;
  float pitch;
  float cp;
  float scale;
  s32 noise;
  s8 frames;

  switch (b->step) {
  case 10:
    goto home;
  case 2:
    b->wobbleYaw = (PlatformRandFloat12() - 1.0f) * 0.62831855f + -0.31415927f;
    b->wobblePitch = 0.31415927f;
    b->wobbleScale = 1.0f;
    b->homeSpeed = 8.0f;
    b->flag1fc = 25;
    b->flag1d2 = 1;
    if (b->owner == NULL) {
      b->step = 9999;
      return;
    }
    b->step = 10;
    goto home;
  case 1:
    pos = b->base.pos;
    vel = b->base.velocity;
    if (ActorBallCheckHit(1.4f, b, pos, vel, 0xbb, 1) == 0) {
      b->travelled = b->travelled + __builtin_sqrtf(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
      if (!(b->travelled <= 150.0f)) {
        b->step++;
      }
      return;
    }
    switch (b->hitType) {
    case 1:
      noise = 1;
      break;
    case 3:
      noise = 3;
      break;
    default:
      noise = 0;
      break;
    }
    ActorNotifyNearestGuardOfNoise(b->vec1e0, noise);
    if (b->flag1d0 != 0) {
      /* dir = normalize(pos - contact), saturated to [-1, 1]; zero length gives 0; w = 0 */
      dir[0] = pos[0] - g_actorBallHitQuery.contact.x;
      dir[1] = pos[1] - g_actorBallHitQuery.contact.y;
      dir[2] = pos[2] - g_actorBallHitQuery.contact.z;
      lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
      k = VfRsq(lenSq);
      if (lenSq == 0.0f) {
        k = 0.0f;
      }
      dir[0] = VfSat1(dir[0] * k);
      dir[1] = VfSat1(dir[1] * k);
      dir[2] = VfSat1(dir[2] * k);
      dir[3] = 0.0f;
      if (ActorBallHitIsFloor(ball)) {
        g_actorBallHitQuery.contact.y = g_actorBallHitQuery.contact.y + 0.2f;
        /* dir = hitNormal + (camera dir - hitNormal) * 0.02 */
        cam = g_gfxActiveCamera->dir;
        dir[0] = b->hitNormal[0] + (cam[0] - b->hitNormal[0]) * 0.02f;
        dir[1] = b->hitNormal[1] + (cam[1] - b->hitNormal[1]) * 0.02f;
        dir[2] = b->hitNormal[2] + (cam[2] - b->hitNormal[2]) * 0.02f;
        dir[3] = b->hitNormal[3] + (cam[3] - b->hitNormal[3]) * 0.02f;
      }
      b->hitPoint[0] = g_actorBallHitQuery.contact.x;
      b->hitPoint[1] = g_actorBallHitQuery.contact.y;
      b->hitPoint[2] = g_actorBallHitQuery.contact.z;
      b->hitPoint[3] = g_actorBallHitQuery.contact.w;
      if (SndHasListener()) {
        SndEmitterCreateAtPos(SndGetListener(), 0x2c00022, b->hitPoint, 0, 1);
      }
      GfxEffectSpawnDirected(g_worldEffectMgr, 7, b->hitPoint, dir);
    }
    b->step++;
    return;
  case 0:
    b->travelled = 0.0f;
    b->flag1d0 = 0;
    b->step++;
    b->inFlight = 1;
    return;
  default:
    b->flag1d3 = 1;
    b->inFlight = 0;
    ActorBallSetState(ball, 0, false);
    return;
  }

home:
  if (b->owner == NULL) {
    b->step = 9999;
    return;
  }
  ActorGetLeftUpperArmPos(b->owner, armPos);
  pos = b->base.pos;
  delta[0] = armPos[0] - pos[0];
  delta[1] = armPos[1] - pos[1];
  delta[2] = armPos[2] - pos[2];
  dist = __builtin_sqrtf(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]);
  if (dist < 3.0f) {
    b->step++;
    return;
  }
  frames = (s8)b->flag1fc - 1;
  b->flag1fc = (u8)frames;
  if (frames < 0) {
    b->step++;
    return;
  }
  yaw = atan2f(delta[2], delta[0]) + b->wobbleYaw * b->wobbleScale;
  pitch = atan2f(delta[1], __builtin_sqrtf(delta[0] * delta[0] + delta[2] * delta[2])) +
          b->wobblePitch * b->wobbleScale;
  cp = __builtin_cosf(pitch);
  delta[0] = __builtin_cosf(yaw) * cp;
  delta[1] = __builtin_sinf(pitch);
  delta[2] = __builtin_sinf(yaw) * cp;
  delta[3] = 0.0f;
  if (b->homeSpeed < 16.0f) {
    b->homeSpeed = b->homeSpeed + 0.2f;
  }
  limit = __builtin_sqrtf(dist) * 1.5f;
  if (!(b->homeSpeed <= limit)) {
    b->homeSpeed = limit;
  }
  delta[0] = delta[0] * b->homeSpeed;
  delta[1] = delta[1] * b->homeSpeed;
  delta[2] = delta[2] * b->homeSpeed;
  if ((((Actor *)b->owner)->flags & 0x20210) == 0) {
    vel = ((Actor *)b->owner)->base.velocity;
    delta[0] = delta[0] + vel[0];
    delta[1] = delta[1] + vel[1];
    delta[2] = delta[2] + vel[2];
  }
  pos[0] = pos[0] + delta[0];
  pos[1] = pos[1] + delta[1];
  pos[2] = pos[2] + delta[2];
  scale = b->wobbleScale - 0.05f;
  b->wobbleScale = scale;
  if (scale < 0.0f) {
    b->wobbleScale = 0.0f;
  }
}
