// bdc 0x08a14514 GmoPlanTakeRec10B
#include "bdc.h"

/* Carves `n` 0x10-byte records (ctor `0x08a14230`) from a model plan. */

void *GmoPlanTakeRec10B(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,(int)sizeof(GmoLayer),n,GmoRec10BCtor);
}

