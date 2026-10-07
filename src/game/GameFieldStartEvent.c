// bdc 0x088c0fe8 GameFieldStartEvent
#include "bdc.h"

/* Starts event script entry `id` of the field task (id 500, `GameFieldCtor`) between characters
   `a` and `b` on its task-470 event object `+0x790` (`GameEvent470StartEntry`); unless
   `keepBalls` is set it also resets the ball list (`ActorBallSetList` with `+0x64c`) and sets
   `+0x3d9`. */

void GameFieldStartEvent(CoreTask *task, s16 id, u8 a, u8 b, bool keepBalls)

{
  GameFieldTask *field = (GameFieldTask *)task;

  GameEvent470StartEntry(field->events,id,a,b);
  if (!keepBalls) {
    ActorBallSetList(field->ballList);
    field->eventView = 1;
  }
  return;
}
