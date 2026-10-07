// bdc 0x0880c59c SaveProfileClearRecordArea
#include "bdc.h"

/* Zero-fills the 0x1e4-byte record area at `+0x6ac` of the profile's save block (`*profile`), if
   the block is attached. Called by `SaveProfileHolderCtor`. */

void SaveProfileClearRecordArea(SaveProfile *self)

{
  if (self->data != (SaveProfileData *)0x0) {
    memset(self->data->recordArea,0,0x1e4);
  }
  return;
}

