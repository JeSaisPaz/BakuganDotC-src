// bdc 0x08a144a4 GmoPlanReserveNodes
#include "bdc.h"

/* Reserves `n` 0xc0-byte node records (alignment 0x40) in pool 0 of a model plan. */

void GmoPlanReserveNodes(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x40,n * 0xc0);
  return;
}

