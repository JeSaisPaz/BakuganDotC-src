// bdc 0x08a129cc GmoImagePlanTakeImages
#include "bdc.h"

/* Carves `n` image records (0x30 bytes) from an image plan, initialised by the ctor at `0x08a128bc`
   (`GmoImagePlanTakeArray`). */

void *GmoImagePlanTakeImages(int n, void *plan)

{
  return GmoImagePlanTakeArray(plan,0,0x10,(int)sizeof(GmoTexTrack),n,GmoImageCtor);
}

