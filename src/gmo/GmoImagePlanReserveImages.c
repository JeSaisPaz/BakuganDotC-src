// bdc 0x08a12924 GmoImagePlanReserveImages
#include "bdc.h"

/* Reserves `n` image records (0x30 bytes, alignment 0x10) in pool 0 of an image plan. */

void GmoImagePlanReserveImages(int n, void *plan)

{
  GmoImagePlanReserve(plan,0,0x10,n * 0x30);
  return;
}

