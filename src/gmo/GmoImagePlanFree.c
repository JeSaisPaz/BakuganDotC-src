// bdc 0x08a10550 GmoImagePlanFree
#include "bdc.h"

/* Releases the three pool blocks of an image-library allocation plan (`GmoImagePlan`)
   (`GmoImageBlockRelease`) and zeroes the whole plan. Does nothing for NULL. */

void GmoImagePlanFree(void *plan)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  int i;

  if (p != NULL) {
    for (i = 0; i < 3; i++) {
      GmoImageBlockRelease(p->blocks[i]);
    }
    memset(p, 0, sizeof(GmoImagePlan));
  }
}
