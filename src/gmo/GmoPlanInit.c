// bdc 0x08a12a34 GmoPlanInit
#include "bdc.h"

/* Zeroes a model-library allocation plan (0x6c bytes, same layout as the image library's plan: 3
   pool blocks, 4 alignment-class totals per pool, carve cursors). */

void GmoPlanInit(void *plan)

{
  if (plan != (void *)0x0) {
    memset(plan,0,0x6c);
    return;
  }
  return;
}

