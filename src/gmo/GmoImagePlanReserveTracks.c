// bdc 0x08a129b0 GmoImagePlanReserveTracks
#include "bdc.h"

/* Forwards to `GmoImagePlanReservePalettes` (same 0x30-byte record size) for animation tracks. */

void GmoImagePlanReserveTracks(int n, void *plan)

{
  GmoImagePlanReservePalettes(n,plan);
  return;
}

