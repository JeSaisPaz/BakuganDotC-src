// bdc 0x08a29f70 ActorCrystalGetAttribute
#include "bdc.h"

/* Crystal override of the battle-unit virtual slot 20 (`+0xa4`, attribute query; base `0x08a29fbc`
   reads `stats[0]`): returns the crystal's attribute `+0x93c`, set from its colour style by
   `ActorCrystalSetStyle`. */

int ActorCrystalGetAttribute(ActorCrystal *self)

{
  return self->attribute;
}

