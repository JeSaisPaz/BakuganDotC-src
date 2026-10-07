// bdc 0x0885fffc BtlBakuganSetHeading
#include "bdc.h"

/* Stores a heading angle (radians) in the Bakugan model's yaw (`base.rot[1]`) and in its input
   block's `heading`. */
void BtlBakuganSetHeading(BtlBakugan *bakugan, float heading)
{
    bakugan->base.rot[1] = heading;
    bakugan->input->heading = heading;
}
