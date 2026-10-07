// bdc 0x08a105d8 GmoImagePlanCommit
#include "bdc.h"

/* Allocates the blocks for the measured totals of an image-library allocation plan
   (`GmoImagePlan`): first releases the plan's three pool blocks, then, depending on the mode byte
   `g_gmoImagePools[1].custom`, either one main pool-0 block sized for pools 0 and 1 together (pool 1
   shares it with an extra reference and its cursors start after pool 0's totals) plus the pool-2
   block, or one block per pool (`GmoImagePlanAllocPool`). On any allocation failure releases
   everything (`GmoImagePlanFree`) and returns 0; returns 0 for a NULL plan and 1 on success. */

int GmoImagePlanCommit(void *plan)

{
  GmoImagePlan *p = plan;
  int sums[4];
  int i;

  if (p == NULL) {
    return 0;
  }
  for (i = 0; i < 3; i++) {
    GmoImageBlockRelease(p->blocks[i]);
    p->blocks[i] = NULL;
  }
  if (g_gmoImagePools[1].custom == 0) {
    for (i = 0; i < 4; i++) {
      sums[i] = p->totals[0][i] + p->totals[1][i];
    }
    if (GmoImagePlanAllocPool(p, 0, sums) != 0 && GmoImagePlanAllocPool(p, 2, NULL) != 0) {
      p->blocks[1] = p->blocks[0];
      GmoImageBlockAddRef(p->blocks[0]);
      p->cursors[1][0] = p->cursors[0][0] + p->totals[0][0];
      p->cursors[1][1] = p->cursors[0][1] + p->totals[0][1];
      p->cursors[1][2] = p->cursors[0][2] + p->totals[0][2];
      p->cursors[1][3] = p->cursors[0][3] + p->totals[0][3];
      return 1;
    }
  } else {
    if (GmoImagePlanAllocPool(p, 0, NULL) != 0 && GmoImagePlanAllocPool(p, 1, NULL) != 0 &&
        GmoImagePlanAllocPool(p, 2, NULL) != 0) {
      return 1;
    }
  }
  GmoImagePlanFree(p);
  return 0;
}
