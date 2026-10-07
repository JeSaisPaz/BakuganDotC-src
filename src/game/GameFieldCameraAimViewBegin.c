// bdc 0x088cc7cc GameFieldCameraAimViewBegin
#include "bdc.h"

/* Starts the field camera's throw aim helper (`cam+0x400`, a `GameFieldCameraSpringCtor` holder
   plus eye/look-at targets `+0x10`/`+0x20` and aim direction `+0x30`): resets the spring to the
   current view `view` (`GameFieldCameraSpringReset`) and the aim direction `+0x30` to (1, 0, 0,
   0). */
void GameFieldCameraAimViewBegin(GameFieldCameraAimView *aim, float *view)
{
  GameFieldCameraSpringReset(&aim->spring, view);
  aim->dir[0] = 1.0f;
  aim->dir[1] = 0.0f;
  aim->dir[2] = 0.0f;
  aim->dir[3] = 0.0f;
}
