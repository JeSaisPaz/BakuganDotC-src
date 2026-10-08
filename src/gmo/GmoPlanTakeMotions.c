// bdc 0x08a144f4 GmoPlanTakeMotions
#include "bdc.h"

/* Carves `n` 0x30-byte motion records (ctor `0x08a14280`) from a model plan; same record type as
   `GmoCreateMotionArray`. */

void *GmoPlanTakeMotions(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,(int)sizeof(GmoMotionInfo),n,GmoMotionRecordCtor);
}

