// bdc 0x0884b228 BtlGetCameraTask
#include "bdc.h"

/* `CoreTaskFind(100)`: returns the camera task object (task id 100), normally after
   `BtlCameraTaskExists` succeeded. */
void *BtlGetCameraTask(void)
{
    return CoreTaskFind(100);
}
