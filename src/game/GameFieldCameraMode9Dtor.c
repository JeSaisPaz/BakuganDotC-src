// bdc 0x088c87d0 GameFieldCameraMode9Dtor
#include "bdc.h"

/* Destructor of the field camera's mode-9 helper (`cam+0x3c0`, pointer to a 0x80-byte spring state
   with a probe at `+0x70`): destroys the probe, frees the state and, when `flags & 1`, the holder. */

void GameFieldCameraMode9Dtor(GameFieldCameraMode9State **holder, u32 flags)
{
  GameFieldCameraMode9State *state;

  if (holder != NULL) {
    state = *holder;
    if (state != NULL) {
      GameFieldCameraProbeDtor(state->probe, 2);
      MemLock();
      MemFree(state, NULL, 0);
      MemUnlock();
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(holder, NULL, 0);
      MemUnlock();
    }
  }
}
