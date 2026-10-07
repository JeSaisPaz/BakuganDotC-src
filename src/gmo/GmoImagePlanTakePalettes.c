// bdc 0x08a129ec GmoImagePlanTakePalettes
#include "bdc.h"

/* Carves `n` palette/track records (0x30 bytes) from an image plan, initialised by the ctor at
   `0x08a1267c`. */

void *GmoImagePlanTakePalettes(int n, void *plan)

{
  return GmoImagePlanTakeArray(plan,0,0x10,0x30,n,GmoImagePaletteCtor);
}

