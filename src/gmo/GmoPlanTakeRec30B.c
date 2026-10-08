// bdc 0x08a14594 GmoPlanTakeRec30B
#include "bdc.h"

/* Carves `n` 0x30-byte records (ctor `0x08a14108`) from a model plan. */

void *GmoPlanTakeRec30B(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,(int)sizeof(GmoMesh),n,GmoRec30BCtor);
}

