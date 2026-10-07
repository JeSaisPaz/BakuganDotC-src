// bdc 0x088bed6c GameFieldSetPlayerPaused
#include "bdc.h"

/* Passes `paused` to the player actor (`ActorFindPlayer`) through `ActorPlayerSetLoopSoundsPaused`, which plays the
   pause/resume sound `0x2c0002b` and toggles player state. */

void GameFieldSetPlayerPaused(CoreTask *task, bool paused)

{
  ActorPlayer *self;
  
  self = ActorFindPlayer();
  if (self != (ActorPlayer *)0x0) {
    ActorPlayerSetLoopSoundsPaused(self,paused);
  }
  return;
}

