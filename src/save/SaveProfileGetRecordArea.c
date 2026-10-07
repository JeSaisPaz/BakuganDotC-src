// bdc 0x0880cd98 SaveProfileGetRecordArea
#include "bdc.h"

/* Returns the address of the record area at `+0x6ac` of the profile's save block, or NULL when no
   block is attached. Also used by `SysUtilSavedataHandlerRequest`. */

void * SaveProfileGetRecordArea(SaveProfile *self)

{
  u8 *area;
  
  area = (u8 *)0x0;
  if (self->data != (SaveProfileData *)0x0) {
    area = self->data->recordArea;
  }
  return area;
}

