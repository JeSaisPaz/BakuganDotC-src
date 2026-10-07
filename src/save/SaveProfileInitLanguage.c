// bdc 0x0880c6a0 SaveProfileInitLanguage
#include "bdc.h"

/* Clears the profile's 4-byte language word (`profile->+8`) and selects language 13 ('system
   default') with `SaveProfileSetLanguage`. Called by `SaveProfileHolderCtor`. */

void SaveProfileInitLanguage(SaveProfile *self)

{
  memset(self->language,0,4);
  SaveProfileSetLanguage(self,0xd);
  return;
}

