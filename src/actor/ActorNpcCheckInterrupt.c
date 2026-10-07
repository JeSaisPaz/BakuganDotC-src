// bdc 0x088e7324 ActorNpcCheckInterrupt
#include "bdc.h"

/* Common pre-check of the NPC state handlers: when the player's input is disabled for an event
   (BtlInput `disabled == 1`), saves the state in `savedState`, plays the idle motion (unless already
   playing) and enters state 9, returning 1; unless `ignorePause`, while the field is paused
   (g_gameFieldCharSet `paused`) starts idle once (`pauseIdle`) and returns 1. Otherwise clears
   `pauseIdle` and returns 0 (the state may run). */

s32 ActorNpcCheckInterrupt(ActorNpc *self, u8 ignorePause)

{
  Actor *player;

  player = (Actor *)ActorFindPlayer();
  if (((BtlInput *)player->input)->disabled == 1) {
    self->savedState = self->aiState;
    if (ActorIsMotionPlaying(&self->base, 0) == 0) {
      ActorPlayMotion(0.2f, self, 0, 1, 0);
    }
    self->aiState = 9;
    return 1;
  }
  if (ignorePause == 0 && g_gameFieldCharSet->paused != 0) {
    if (self->pauseIdle == 0) {
      ActorPlayMotion(0.2f, self, 0, 1, 0);
      self->pauseIdle = 1;
    }
    return 1;
  }
  self->pauseIdle = 0;
  return 0;
}
