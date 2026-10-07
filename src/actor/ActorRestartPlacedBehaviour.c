// bdc 0x088df3f0 ActorRestartPlacedBehaviour
#include "bdc.h"

/* Selects behaviour 0, plays the placed idle motion (`ActorPlayPlacedMotion`), clears the
   `disabled` byte of the input helper `+0x164` and switches to behaviour 9 when the actor has a
   route (`+0x35c`). Used by event commands. */

void ActorRestartPlacedBehaviour(Actor *self)
{
  ActorSetStateBase(self, 0, 0);
  ActorPlayPlacedMotion(self, 0);
  if (self->input != NULL) {
    ((BtlInput *)self->input)->disabled = 0;
  }
  if (self->route != NULL) {
    ActorSetStateBase(self, 9, 0);
  }
}
