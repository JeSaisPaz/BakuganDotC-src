// bdc 0x089c4f2c SndDecOutSetDecodeEnabled
#include "bdc.h"

/* Sets `decodeEnabled` under the lock: when 0, `SndDecOutThreadStep` outputs silence instead of
   decoding ATRAC data. */

void SndDecOutSetDecodeEnabled(SndDecOut *dec, u8 enabled)

{
  CoreLockAcquire(dec->lock);
  dec->decodeEnabled = enabled;
  CoreLockRelease(dec->lock);
  return;
}

