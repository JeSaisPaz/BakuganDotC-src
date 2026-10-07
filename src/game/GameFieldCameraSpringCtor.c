// bdc 0x088cce04 GameFieldCameraSpringCtor
#include "bdc.h"

/* Constructor of a camera spring holder (pointer to a 0x40-byte state: eye velocity `+0x00`,
   look-at velocity `+0x10`, eye `+0x20`, look-at `+0x30`): allocates the state from the low heap
   and stores the pointer in `*holder`. Base of the aim view (`GameFieldCameraAimViewCtor`) and
   head view (`GameFieldCameraHeadViewCtor`) helpers of the field camera. */

void GameFieldCameraSpringCtor(void **holder)

{
  bool fromLow;
  void *state;
  void *result;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  state = MemAlloc(sizeof(GameFieldCameraSpring), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  result = NULL;
  if (state != NULL) {
    result = state;
  }
  *holder = result;
}
