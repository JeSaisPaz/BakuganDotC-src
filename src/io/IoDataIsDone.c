// bdc 0x089fbbb0 IoDataIsDone
#include "bdc.h"

/* Returns the done flag (`+0x38`) of a data request (`COData`); while not done, wakes the data
   thread when it sleeps. */

bool IoDataIsDone(IoData *self)
{
    u8 done = self->done;

    if (done == 0 && BootIsThreadSleeping(4) != 0) {
        BootWakeupThread(4);
    }
    return done;
}
