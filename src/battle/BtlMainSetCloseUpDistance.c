// bdc 0x0884c2dc BtlMainSetCloseUpDistance
#include "bdc.h"

/* Sets the close-up distance of the battle main task's embedded camera (`camera.closeUpDistance`,
   which `BtlCameraStartCloseUp` initialises to 100). */
void BtlMainSetCloseUpDistance(float dist, BtlMain *self)
{
    self->camera.closeUpDistance = dist;
}
