// bdc 0x08a1441c GmoPlanReserveRec20
#include "bdc.h"

/* Reserves `n` 0x20-byte records in pool 0 of a model plan. */

void GmoPlanReserveRec20(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n * (int)sizeof(GmoInstance));
  return;
}

