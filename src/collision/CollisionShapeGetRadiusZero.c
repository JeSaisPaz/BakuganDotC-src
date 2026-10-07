// bdc 0x08a29ec8 CollisionShapeGetRadiusZero
#include "bdc.h"

/* Default get-radius virtual (entry 11, offset `+0x5c`) of the ray (`0x08af5504`), segment
   (`0x08af5564`) and box (`0x08af5684`) collision shapes: returns 0.0. */
float CollisionShapeGetRadiusZero(void *shape)
{
    (void)shape;
    return 0.0f;
}
