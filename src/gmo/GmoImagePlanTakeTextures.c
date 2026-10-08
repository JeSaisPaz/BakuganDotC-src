// bdc 0x08a12a0c GmoImagePlanTakeTextures
#include "bdc.h"

/* Carves `n` texture records (0x40 bytes) from an image plan, initialised by `GmoTextureInit`. */

void *GmoImagePlanTakeTextures(int n, void *plan)

{
  return GmoImagePlanTakeArray(plan,0,0x10,(int)sizeof(GmoTexture),n,GmoTextureInit);
}

