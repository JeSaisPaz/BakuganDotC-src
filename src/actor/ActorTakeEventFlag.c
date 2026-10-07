// bdc 0x088deaac ActorTakeEventFlag
#include "bdc.h"

/* Reads and clears the byte at `actor + 0x340` and returns the old value: a one-shot event flag
   that something else raises on the actor. Used by the script opcode `ScriptOpJumpIfActorEvent`
   ("branch if the object was triggered since last time"). */

u8 ActorTakeEventFlag(Actor *self)
{
  u8 flag;

  flag = self->eventFlag;
  self->eventFlag = 0;
  return flag;
}
