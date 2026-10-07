// bdc 0x0890781c BtlDemoScenePlayerStatePlay
#include "bdc.h"

/* State 2 of the battle demo scene player task (`BtlDemoScenePlayer`, task id 0x66,
   `BtlDemoScenePlayerCtor`): while `finished` is 0, runs the frame's events
   (`BtlDemoSceneRunEvents`), the camera keys (`BtlDemoScenePlayerPlayCameraKeys`) and the node
   tracking (`BtlDemoSceneTrackSphere`); otherwise does nothing. */
void BtlDemoScenePlayerStatePlay(BtlDemoScenePlayer *task)
{
    if (task->finished == 0) {
        BtlDemoSceneRunEvents(task);
        BtlDemoScenePlayerPlayCameraKeys(task);
        BtlDemoSceneTrackSphere(task);
    }
}
