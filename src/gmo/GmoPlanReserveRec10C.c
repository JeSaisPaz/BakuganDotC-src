// bdc 0x08a143f0 GmoPlanReserveRec10C
#include "bdc.h"

/* Reserves `n` 0x10-byte records in pool 0 of a model plan (model measurer). */

void GmoPlanReserveRec10C(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n * (int)sizeof(GmoMaterial));
  return;
}

