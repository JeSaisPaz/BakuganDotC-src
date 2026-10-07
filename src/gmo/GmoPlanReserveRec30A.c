// bdc 0x08a14368 GmoPlanReserveRec30A
#include "bdc.h"

/* Reserves `n` 0x30-byte records (alignment 0x10) in pool 0 of a model plan (model measurer
   `GmoModelMeasure`). */

void GmoPlanReserveRec30A(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n * 0x30);
  return;
}

