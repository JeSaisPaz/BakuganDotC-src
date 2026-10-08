// bdc 0x088c0e10 GameFieldHandlePlayerCaught
#include "bdc.h"

/* Handles the player being caught for the field task (id 500, `GameFieldCtor`): for slot `slot`
   (not 0xff; the catching guard's character slot) reads the follow-up event id `caughtEventId` from that
   placed actor's placement (`g_gameFieldCharSet->actors[slot]` → `+0x350` → `+0x30`), cancels a throw in
   progress (`ActorPlayerDropBall`), freezes the player's actor (`GameFieldCharSetFreezeActor`)
   unless the player is in state 9, and resets the camera behind the player
   (`GameFieldCameraReset`). */

void GameFieldHandlePlayerCaught(CoreTask *task, u8 slot)

{
  GameFieldTask *field = (GameFieldTask *)task;
  GameFieldCharSet *set = g_gameFieldCharSet;
  ActorPlayer *self;

  if (slot != 0xff) {
    Actor *a = (Actor *)set->actors[slot];

    if (a == 0) {
      self = (ActorPlayer *)g_gameFieldCharSet->actors[0];
      goto check;
    }
    field->caughtEventId = ((GameFieldPlacedChar *)a->placement)->eventId;
    set = g_gameFieldCharSet;
  }
  self = (ActorPlayer *)set->actors[0];
check:
  if (self != 0) {
    ActorPlayerDropBall(self);
    if (self->base.state != 9) {
      GameFieldCharSetFreezeActor(field->charSet,0);
    }
  }
  GameFieldCameraReset((GameFieldCamera *)((GameFieldTask *)task)->camera,'\0','\0');
  return;
}
