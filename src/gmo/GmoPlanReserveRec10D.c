// bdc 0x08a14478 GmoPlanReserveRec10D
#include "bdc.h"

/* Reserves `n` 0x10-byte records in pool 0 of a model plan (model measurer). */

void GmoPlanReserveRec10D(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n * (int)sizeof(GmoPart));
  return;
}

