// bdc 0x08a12984 GmoImagePlanReserveTextures
#include "bdc.h"

/* Reserves `n` texture records (0x40 bytes, alignment 0x10) in pool 0 of an image plan. */

void GmoImagePlanReserveTextures(int n, void *plan)

{
  GmoImagePlanReserve(plan,0,0x10,n * (int)sizeof(GmoTexture));
  return;
}

