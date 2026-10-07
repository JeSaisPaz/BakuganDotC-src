// bdc 0x08a14448 GmoPlanReserveRec30B
#include "bdc.h"

/* Reserves `n` 0x30-byte records in pool 0 of a model plan (model measurer). */

void GmoPlanReserveRec30B(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n * 0x30);
  return;
}

