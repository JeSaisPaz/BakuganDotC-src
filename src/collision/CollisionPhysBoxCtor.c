// bdc 0x089e67ec CollisionPhysBoxCtor
#include "bdc.h"

/* Constructor of a physics box (8 corner particles, Verlet-style: positions `*box`, velocities
   `box[1]`, previous positions `box[2]`, 28 edge rest lengths `box[3..]`, per-corner pin bytes
   `+0x7c`, vtable `0x08af54ec` at `+0x164`): sets the vtable at `+0x164`, clears the particle
   buffer pointer and `+0x118`. */

CollisionPhysBox *CollisionPhysBoxCtor(CollisionPhysBox *self)

{
  self->vtbl = g_collisionPhysBoxVtbl;
  self->counter118 = 0;
  self->pos = (ScePspFVector4 *)0x0;
  return self;
}

