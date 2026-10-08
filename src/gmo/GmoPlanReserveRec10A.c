// bdc 0x08a1433c GmoPlanReserveRec10A
#include "bdc.h"

/* Reserves `n` 0x10-byte records (alignment 0x10) in pool 0 of a model plan; first of the
   per-chunk-type reservations made by the model measurer `GmoModelMeasure`. */

void GmoPlanReserveRec10A(int n, void *plan)

{
  GmoPlanReserve(plan,0,0x10,n * (int)sizeof(GmoMotionTrack));
  return;
}

