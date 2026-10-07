// bdc 0x088ccf10 GameFieldCameraSpringReset
#include "bdc.h"

/* Resets a camera spring holder (pointer to a 0x40-byte state: eye velocity `+0x00`, look-at
   velocity `+0x10`, eye `+0x20`, look-at `+0x30`): clears both velocities and sets the eye/look-at
   positions from `view[0]`/`view[1]` (two vec4s). */

void GameFieldCameraSpringReset(void **holder, float *view)
{
  float *state;
  int i;

  state = (float *)*holder;
  for (i = 0; i < 4; i++) {
    state[i] = 0.0f;
  }
  state = (float *)*holder;
  for (i = 0; i < 4; i++) {
    state[4 + i] = 0.0f;
  }
  state = (float *)*holder;
  for (i = 0; i < 4; i++) {
    state[8 + i] = view[i];
  }
  state = (float *)*holder;
  for (i = 0; i < 4; i++) {
    state[12 + i] = view[4 + i];
  }
}
