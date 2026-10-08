// bdc 0x08a14398 GmoPlanReserveRec10B
#include "bdc.h"

/* Reserves `n` 0x10-byte records in pool 0 of a model plan (model measurer). */

void GmoPlanReserveRec10B(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n * (int)sizeof(GmoLayer));
  return;
}

