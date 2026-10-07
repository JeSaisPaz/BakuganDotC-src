// bdc 0x089cbd30 SysUtilInitDialogCommon
#include "bdc.h"

/* Initialises the `pspUtilityDialogCommon` header at the start of a utility parameter block: stores
   `params` in `*slot`, zeroes `size` bytes, then sets `size`, `language` (from
   `SaveProfileGetLanguage`), `buttonSwap = 1` (cross confirms) and the thread priorities graphics
   0x11, access 0x13, font 0x12, sound 0x10. */

void SysUtilInitDialogCommon(void **slot, void *params, u32 size)

{
  pspUtilityDialogCommon *common = (pspUtilityDialogCommon *)params;
  SaveProfile *self;

  *slot = params;
  memset(params,0,size);
  common->size = size;
  self = SaveGetProfile();
  common->language = SaveProfileGetLanguage(self);
  common->buttonSwap = 1;
  common->graphicsThread = 0x11;
  common->accessThread = 0x13;
  common->fontThread = 0x12;
  common->soundThread = 0x10;
}
