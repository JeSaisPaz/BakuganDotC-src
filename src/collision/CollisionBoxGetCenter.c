// bdc 0x08a32480 CollisionBoxGetCenter
#include "bdc.h"

/* Get-centre method of the box shape (vtable `0x08af5684` entry 8, offset `+0x44`): returns `box +
   0x60`, the translation row of its transform matrix (`+0x30`), i.e. the box centre. */

ScePspFVector4 *CollisionBoxGetCenter(CollisionBox *self)

{
  return &(self->transform).w;
}

