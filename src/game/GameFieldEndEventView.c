// bdc 0x088bf300 GameFieldEndEventView
#include "bdc.h"

/* Ends an event view of the field task (id 500, `GameFieldCtor`): restarts the placed actors of
   the character-set manager `charSet` (`GameFieldCharSetRestartAll`), blends the camera back to
   the follow view (`GameFieldCameraBeginBlendToFollow`) when its mode (`cameraMode`) is not 0,
   sets `eventViewTimer = 16` and clears `eventView`. */

void GameFieldEndEventView(CoreTask *self)
{
  GameFieldTask *task = (GameFieldTask *)self;

  GameFieldCharSetRestartAll(task->charSet);
  if (task->cameraMode != 0) {
    GameFieldCameraBeginBlendToFollow((GameFieldCamera *)task->camera);
  }
  task->eventViewTimer = 16;
  task->eventView = 0;
}
