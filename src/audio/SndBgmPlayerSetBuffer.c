// bdc 0x089c31fc SndBgmPlayerSetBuffer
#include "bdc.h"

/* Chooses where the player's `.at3` file image lives: `buffer` is either 0 / 1 (let CODataMng
   allocate from the heap top / bottom; stored as the default direction `loadFromLow = buffer != 0`)
   or a pointer to a caller-supplied block (`externalBuffer = 1`), typically PSP-2000 volatile
   memory. Also records whether the pointer lies in volatile memory (`bufferVolatile`, via
   `CorePowerIsVolatilePtr`) and clears `bufferReleased`. */

void SndBgmPlayerSetBuffer(SndBgmPlayer *player, void *buffer)

{
  int ok;
  CorePower *power;
  u8 isVolatile;
  uintptr_t bufferValue;

  isVolatile = 0;
  ok = CorePowerIsInitialized();
  if (ok != 0) {
    power = CorePowerGet();
    ok = CorePowerIsVolatilePtr(power,buffer);
    if (ok != 0) {
      isVolatile = 1;
    }
  }
  CoreLockAcquire(player->lock);
  player->bufferVolatile = isVolatile;
  player->bufferReleased = 0;
  player->buffer = buffer;
  bufferValue = (uintptr_t)buffer;
  if (bufferValue == 0 || bufferValue == 1) {
    player->loadFromLow = bufferValue != 0;
    player->externalBuffer = 0;
  }
  else {
    player->externalBuffer = 1;
  }
  CoreLockRelease(player->lock);
  return;
}
