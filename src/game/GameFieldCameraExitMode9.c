// bdc 0x088bd5c8 GameFieldCameraExitMode9
#include "bdc.h"

/* Mode 9 exit handler of the field camera (`GameFieldCameraCtor`): fades the field HUD (task
   3001, helper `+0x6c`) back in (`UiFieldHudFaderFadeIn`) and restores the player's alpha
   (`GameFieldCameraMode9Exit`). */

void GameFieldCameraExitMode9(GameFieldCamera *cam)

{
  UiFieldHud *hud = (UiFieldHud *)CoreTaskFind(0xbb9);

  UiFieldHudFaderFadeIn(&hud->fader);
  GameFieldCameraMode9Exit((void **)cam->mode9);
  return;
}
