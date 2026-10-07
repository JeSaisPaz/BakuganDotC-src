// bdc 0x0884839c BtlCameraStartCloseUp
#include "bdc.h"

/* Starts a camera close-up on the controller: frame counter = `frames`, both blend factors =
   `blend`, close-up parameter = `param`, distance = 100.0 (consumed by `BtlCameraApplyCloseUp`). */
void BtlCameraStartCloseUp(float blend, float param, BtlCamera *camera, int frames)
{
    camera->closeUpFrames = frames;
    camera->closeUpBlendA = blend;
    camera->closeUpBlendB = blend;
    camera->closeUpParam = param;
    camera->closeUpDistance = 100.0f;
}
