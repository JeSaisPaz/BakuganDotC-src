// bdc 0x08a323d0 CollisionRayGetCenter
#include "bdc.h"

/* Get-centre method of the ray shape (vtable `0x08af5504` entry 8, offset `+0x44`): returns `ray +
   0x10`, its origin. */

ScePspFVector4 *CollisionRayGetCenter(CollisionRayShape *ray)

{
  return &ray->origin;
}
