// bdc 0x088e2ab0 ActorPlayerStateWalk
#include "bdc.h"

/* State 1 (walk/run) handler of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550 bytes,
   constructor `ActorPlayerCtor`, vtable `0x08af38e4`) (vtable slot 21). Command bit `0x400000` in
   `motion` first switches to state 7 (throw), else `ActorPlayerWantsGauntletView` switches to state
   0xb; then, without the move command bit 1, it returns to state 0. With it, the stick magnitude
   (`input->stick` x/y/z) picks run (motion 1, target speed 2.4) or, below 0.97,
   slow walk (motion 0x2e, target 0.48) into `walkMotion`; `walkSpeed` eases toward the target by 0.2
   per frame (with flag `0x20000` it instead decays by 0.8 while above 0.3). It plays the motion
   (`ActorPlayMotion`), turns toward the stick heading (`ActorTurnToward`, rate 0.3) and sets the
   velocity x/z from the heading (cos/sin) scaled by `walkSpeed` times a factor that drops while
   turning sharply. Every 7 (run) or 15 (walk) frames (`stepCounter`) it starts one of the six
   footstep sounds `0x2c00001..0x2c00006` picked by the VFPU random generator
   (`SndObjectAddEmitter`). */

void ActorPlayerStateWalk(ActorPlayer *self)

{
  Actor *actor = &self->base;
  BtlInput *input;
  float *stick;
  float mag;
  float heading;
  float speed;
  float target;
  float turn;
  float *vel;
  s32 slot;
  s32 step;
  u32 rnd;

  if ((actor->motion & 0x400000U) != 0) {
    ActorSetState(actor, 7, 0);
  }
  else if (ActorPlayerWantsGauntletView(self)) {
    ActorSetState(actor, 0xb, 0);
  }
  if ((actor->motion & 1) == 0) {
    ActorSetState(actor, 0, 0);
    return;
  }
  target = 2.4f;
  slot = 1;
  stick = ((BtlInput *)actor->input)->stick;
  mag = __builtin_sqrtf(stick[0] * stick[0] + stick[1] * stick[1] + stick[2] * stick[2]);
  if (mag < 0.97f) {
    target = target * 0.2f;
    slot = 0x2e;
  }
  actor->walkMotion = slot;
  if ((actor->flags & 0x20000) != 0) {
    if (!(actor->walkSpeed <= 0.3f)) {
      actor->walkSpeed = actor->walkSpeed * 0.8f;
    }
  }
  else {
    actor->walkSpeed = actor->walkSpeed + (target - actor->walkSpeed) * 0.2f;
  }
  ActorPlayMotion(0.2f, self, slot, 1, 0);
  turn = ActorTurnToward(((BtlInput *)actor->input)->heading, 0.3f, 0.0f, self);
  turn = __builtin_fabsf(turn) + 0.942477882f;
  if (!(turn <= 3.14159274f)) {
    turn = 3.14159274f;
  }
  turn = (3.14159274f - turn) * 0.477464825f;
  if (!(turn <= 1.0f)) {
    turn = 1.0f;
  }
  vel = actor->base.velocity;
  input = (BtlInput *)actor->input;
  heading = input->heading;
  speed = actor->walkSpeed * turn;
  /* vrot of heading * 2/pi (bank S703) quarter turns: (cos, 0, sin) scaled by speed */
  vel[0] = __builtin_cosf(heading) * speed;
  vel[2] = __builtin_sinf(heading) * speed;
  step = actor->stepCounter - 1;
  actor->stepCounter = step;
  if (step != 0) {
    return;
  }
  actor->stepCounter = (slot == 0x2e) ? 0xf : 7;
  rnd = PlatformRandU32();
  SndObjectAddEmitter(actor->base.sound, (s32)(((rnd >> 16) * 6) >> 16) + 0x2c00001, 0, 0);
}
