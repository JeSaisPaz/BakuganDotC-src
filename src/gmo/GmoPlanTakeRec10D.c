// bdc 0x08a145b4 GmoPlanTakeRec10D
#include "bdc.h"

/* Carves `n` 0x10-byte records (ctor `0x08a140e4`) from a model plan. */

void *GmoPlanTakeRec10D(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,(int)sizeof(GmoPart),n,GmoRec10DCtor);
}

