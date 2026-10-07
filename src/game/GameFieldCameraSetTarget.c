// bdc 0x088b9e64 GameFieldCameraSetTarget
#include "bdc.h"

/* Sets the follow target actor of the field camera (`GameFieldCameraCtor`). */
void GameFieldCameraSetTarget(GameFieldCamera *cam, void *target)
{
    cam->target = target;
}
