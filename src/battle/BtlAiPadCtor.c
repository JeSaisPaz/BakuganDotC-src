// bdc 0x088991bc BtlAiPadCtor
#include "bdc.h"

/* Constructor of the virtual pad of `BtlAi`: builds both input controllers `cur` and
   `prev` (`BtlInputCtorForUnit` with no unit), clears the owner, the stick axes, magnitude,
   `stickF0` and heading, zeroes `vec` (the bank constant C720 = (0,0,0,0)), and clears the
   pending AI action bits of both controllers. Returns `self`. */
BtlAiPad *BtlAiPadCtor(BtlAiPad *self)
{
  BtlInputCtorForUnit(&self->cur, NULL);
  BtlInputCtorForUnit(&self->prev, NULL);
  self->owner = NULL;
  self->stickX = 0.0f;
  self->stickY = 0.0f;
  self->stickMagnitude = 0.0f;
  self->stickF0 = 0.0f;
  self->stickHeading = 0.0f;
  self->vec[0] = 0.0f;
  self->vec[1] = 0.0f;
  self->vec[2] = 0.0f;
  self->vec[3] = 0.0f;
  self->cur.aiActions = 0;
  self->prev.aiActions = 0;
  return self;
}
