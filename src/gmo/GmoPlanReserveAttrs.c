// bdc 0x08a143c4 GmoPlanReserveAttrs
#include "bdc.h"

/* Reserves `n` 0x40-byte material attribute records in pool 0 of a model plan. */

void GmoPlanReserveAttrs(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n << 6);
  return;
}

