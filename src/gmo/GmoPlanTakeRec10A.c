// bdc 0x08a144d4 GmoPlanTakeRec10A
#include "bdc.h"

/* Carves `n` 0x10-byte records (ctor `0x08a14308`) from a model plan. */

void *GmoPlanTakeRec10A(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,0x10,n,GmoRec10ACtor);
}

