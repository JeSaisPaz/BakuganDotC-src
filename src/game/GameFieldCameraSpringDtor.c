// bdc 0x088cce80 GameFieldCameraSpringDtor
#include "bdc.h"

/* Destructor of a camera spring holder (pointer to a 0x40-byte state: eye velocity `+0x00`, look-at
   velocity `+0x10`, eye `+0x20`, look-at `+0x30`): frees the state and, when `flags & 1`, the
   holder itself. */

void GameFieldCameraSpringDtor(void **holder, u32 flags)

{
  void *ptr;
  
  if (holder != (void **)0x0) {
    ptr = *holder;
    if (ptr != (void *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(holder,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

