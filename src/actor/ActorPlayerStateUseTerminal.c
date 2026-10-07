// bdc 0x088e2d58 ActorPlayerStateUseTerminal
#include "bdc.h"

/* State 4 handler of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550 bytes, constructor
   `ActorPlayerCtor`, vtable `0x08af38e4`, slot 24): the player uses a stage terminal. Sub-step
   `waitTimer` (`+0x324`): 0/1 turn toward the terminal (`GameStageGetTerminalPosition``(out, 1)`,
   `ActorTurnToward`; step 0 plays the walk motion while more than 0.8 rad off, step 1 waits until
   within 0.01 rad, then starts the idle motion with a 4-frame `stateDelay`, or 1 frame when it already
   plays), 2 counts `stateDelay` below 0, 3 waits for an outside step change, 4 waits until task 380
   (`UiEmptyScreen380Ctor`) is gone and continues only when script global 3 is 1 (else state 0), 5
   snaps the player onto the terminal (`GameStageGetTerminalPosition``(out, 0)`, 4-float copy) and
   switches the camera to the terminal view (`GameFieldCameraBeginTerminalView`), 6 passes, 7 opens
   the terminal task 390 (`UiScreen390Ctor`), 8 waits until it closes, 9 passes, and any later
   step returns to state 0. Without a camera (`camera`) it drops to state 0 at once. */

void ActorPlayerStateUseTerminal(ActorPlayer *self)

{
  Actor *actor = &self->base;
  float pos[4] __attribute__((aligned(16)));
  float aim0[4] __attribute__((aligned(16)));
  float aim1[4] __attribute__((aligned(16)));
  float turn;

  if (actor->camera == NULL) {
    ActorSetState(actor, 0, 0);
    return;
  }
  switch (actor->waitTimer) {
  case 0:
    GameStageGetTerminalPosition(aim0, 1);
    turn = ActorTurnToward(atan2f(aim0[2] - actor->base.pos[2], aim0[0] - actor->base.pos[0]), 0.3f,
                           0.0f, self);
    if (!(fabsf(turn) <= 0.8f)) {
      ActorPlayMotion(0.2f, self, 1, 1, 0);
    }
    actor->waitTimer = actor->waitTimer + 1;
    break;
  case 1:
    GameStageGetTerminalPosition(aim1, 1);
    turn = ActorTurnToward(atan2f(aim1[2] - actor->base.pos[2], aim1[0] - actor->base.pos[0]), 0.3f,
                           0.0f, self);
    if (fabsf(turn) < 0.00999999978f) {
      if (ActorIsMotionPlaying(actor, 0) == 0) {
        ActorPlayMotion(0.2f, self, 0, 1, 0);
        actor->stateDelay = 4;
      }
      else {
        actor->stateDelay = 1;
      }
      actor->waitTimer = actor->waitTimer + 1;
    }
    break;
  case 2:
    actor->stateDelay = actor->stateDelay - 1;
    if (actor->stateDelay < 0) {
      actor->waitTimer = actor->waitTimer + 1;
    }
    break;
  case 3:
    break;
  case 4:
    if (CoreTaskExists(0x17c) == 0) {
      if (g_scriptGlobalVars[3] == 1) {
        actor->waitTimer = actor->waitTimer + 1;
      }
      else {
        ActorSetState(actor, 0, 0);
      }
    }
    break;
  case 5:
    GameStageGetTerminalPosition(pos, 0);
    actor->base.pos[0] = pos[0];
    actor->base.pos[1] = pos[1];
    actor->base.pos[2] = pos[2];
    actor->base.pos[3] = pos[3];
    GameFieldCameraBeginTerminalView(actor->camera);
    actor->waitTimer = actor->waitTimer + 1;
    break;
  case 6:
    actor->waitTimer = actor->waitTimer + 1;
    break;
  case 7:
    CoreTaskCreate(0x186, 100);
    actor->waitTimer = actor->waitTimer + 1;
    break;
  case 8:
    if (CoreTaskExists(0x186) == 0) {
      actor->waitTimer = actor->waitTimer + 1;
    }
    break;
  case 9:
    actor->waitTimer = actor->waitTimer + 1;
    break;
  default:
    ActorSetState(actor, 0, 0);
    break;
  }
}
