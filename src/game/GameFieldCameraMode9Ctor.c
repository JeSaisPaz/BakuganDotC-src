// bdc 0x088c8740 GameFieldCameraMode9Ctor
#include "bdc.h"

/* Constructor of the field camera's mode-9 helper (`cam+0x3c0`, pointer to a 0x80-byte spring state
   with a probe at `+0x70`): allocates the 0x80-byte state from the low heap and constructs the
   embedded probe (`GameFieldCameraProbeCtor`). */

GameFieldCameraMode9State **GameFieldCameraMode9Ctor(GameFieldCameraMode9State **holder)
{
  bool fromLow;
  GameFieldCameraMode9State *state;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  state = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (state != NULL) {
    GameFieldCameraProbeCtor(state->probe);
  }
  *holder = state;
  return holder;
}
