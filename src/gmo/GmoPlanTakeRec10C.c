// bdc 0x08a14554 GmoPlanTakeRec10C
#include "bdc.h"

/* Carves `n` 0x10-byte records (ctor `0x08a14198`) from a model plan. */

void *GmoPlanTakeRec10C(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,0x10,n,GmoRec10CCtor);
}

