// bdc 0x08a2a604 BtlAiParamsReturn40
#include "bdc.h"

/* Default per-species AI parameter getter (entry 7, fn at `+0x3c`, base vtable `0x08af6280`):
   returns the constant 40.0. */

float BtlAiParamsReturn40(BtlAiParams *self)
{
  (void)self;
  return 40.0f;
}
