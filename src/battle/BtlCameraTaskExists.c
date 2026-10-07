// bdc 0x0884b208 BtlCameraTaskExists
#include "bdc.h"

/* `CoreTaskExists(100)`: tests whether the camera task (task id 100) is alive; ~60 callers use
   this as 'a scene with a camera is running'. */
int BtlCameraTaskExists(void)
{
    return CoreTaskExists(100);
}
