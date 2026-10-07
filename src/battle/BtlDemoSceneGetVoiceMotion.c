// bdc 0x08906378 BtlDemoSceneGetVoiceMotion
#include "bdc.h"

/* Returns entry `index` of `g_btlDemoVoiceMotions` for `index` < 2, or 0 for other indices and
   for -1 entries. */
int BtlDemoSceneGetVoiceMotion(void *task, int index)
{
    int motion = -1;

    (void)task;
    if (index < 2) {
        motion = g_btlDemoVoiceMotions[index];
    }
    if (motion == -1) {
        motion = 0;
    }
    return motion;
}
