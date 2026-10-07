// bdc 0x089d051c NetCharaIsConnected
#include "bdc.h"

/* Returns the character's connected byte (`+1`) under its lock. `NetCharaMgrUpdate` uses it to
   find dropped peers. */

u8 NetCharaIsConnected(NetChara *self)

{
  u8 connected;
  
  CoreLockAcquire(self->lock);
  connected = self->connected;
  CoreLockRelease(self->lock);
  return connected;
}

