// bdc 0x08a12954 GmoImagePlanReservePalettes
#include "bdc.h"

/* Reserves `n` palette/track records (0x30 bytes, alignment 0x10) in pool 0 of an image plan. */

void GmoImagePlanReservePalettes(int n, void *plan)

{
  GmoImagePlanReserve(plan,0,0x10,n * (int)sizeof(GmoImage));
  return;
}

