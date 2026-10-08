// bdc 0x08a145f4 GmoCreateMotionArray
#include "bdc.h"

/* Creates a standalone array of `n` 0x30-byte motion records: measures, commits and carves a
   private plan (`GmoPlanReserve`, `GmoPlanCommit`, `GmoPlanTakeArray` with ctor
   `0x08a14280`), then drops the plan's own references. Returns the array, or NULL if the
   commit fails. */

void *GmoCreateMotionArray(int n)
{
  int plan[28];
  int committed;
  void *array;

  GmoPlanInit(plan);
  GmoPlanReserve(plan, 0, 0x10, n * (int)sizeof(GmoMotionInfo));
  committed = GmoPlanCommit(plan);
  array = (void *)0x0;
  if (committed != 0) {
    array = GmoPlanTakeArray(plan, 0, 0x10, (int)sizeof(GmoMotionInfo), n, GmoMotionRecordCtor);
    GmoPlanFree(plan);
  }
  return array;
}
