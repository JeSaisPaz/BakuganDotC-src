// bdc 0x08a14574 GmoPlanTakeRec20
#include "bdc.h"

/* Carves `n` 0x20-byte records (ctor `0x08a14158`) from a model plan. */

void *GmoPlanTakeRec20(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,0x20,n,GmoRec20Ctor);
}

