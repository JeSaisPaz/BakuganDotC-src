// bdc 0x088c1044 GameFieldReturnToLocation
#include "bdc.h"

/* Returns the field task (id 500, `GameFieldCtor`) to the saved location: restores it
   (`GameFieldRestoreReturnLocation`), places the player (`ActorApplyPlacement`), resets the party
   (`GameFieldCharSetFreezeAll`), snaps the camera (`GameFieldCameraReset`), starts the area change
   (`GameFieldStartEvent`), closes the HUD and sets `returnedFlag`. */

void GameFieldReturnToLocation(CoreTask *task, s16 id, u8 b)

{
  GameFieldTask *field = (GameFieldTask *)task;

  GameFieldRestoreReturnLocation(task);
  ActorApplyPlacement((Actor *)g_gameFieldCharSet->actors[0]);
  GameFieldCharSetFreezeAll(field->charSet);
  GameFieldCameraReset((GameFieldCamera *)(task + 2),'\x01','\0');
  GameFieldStartEvent(task,id,'\0',b,false);
  GameFieldCloseHud();
  field->returnedFlag = 1;
  return;
}
