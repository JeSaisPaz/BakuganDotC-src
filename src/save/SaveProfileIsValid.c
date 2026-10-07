// bdc 0x0880d448 SaveProfileIsValid
#include "bdc.h"

/* Validates a profile's save block: true when the block is attached, its word 0 is 1 and word 1
   equals `SaveGetDataBlockSize() + 2` (the stamps `SaveProfileReset` writes). Used by
   `SaveLoadTaskUpdate` to reject corrupt or foreign save data. */

bool SaveProfileIsValid(SaveProfile *self)
{
  SaveProfileData *data;
  bool valid;
  int stamp;

  data = self->data;
  valid = false;
  if ((data != (SaveProfileData *)0x0) && (data->magic == 1) &&
      (stamp = data->sizeStamp, stamp == SaveGetDataBlockSize() + 2)) {
    valid = true;
  }
  return valid;
}
