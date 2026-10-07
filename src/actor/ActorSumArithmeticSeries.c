// bdc 0x088e2718 ActorSumArithmeticSeries
#include "bdc.h"

/* Returns `sum(first + i*step, i = 0..count-1)` with a loop (used to integrate the throw arc). */

s32 ActorSumArithmeticSeries(s32 first, s32 step, s32 count)
{
  s32 sum;
  s32 i;

  sum = 0;
  for (i = 0; i < count; i++) {
    sum += first;
    first += step;
  }
  return sum;
}
