// bdc 0x0884c280 BtlStartCameraCloseUp
#include "bdc.h"

/* Finds the battle main task (id 100) and starts a camera close-up on it
   (`BtlMainStartCameraCloseUp`); does nothing when the task does not exist. */
void BtlStartCameraCloseUp(float blend, float param, int frames)
{
    BtlMain *main = CoreTaskFind(100);

    if (main != NULL) {
        BtlMainStartCameraCloseUp(blend, param, main, frames);
    }
}
