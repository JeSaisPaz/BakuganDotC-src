// bdc 0x0884b390 BtlStopBgm
#include "bdc.h"

/* Cancels and stops both BGM channels: channel 0 fading over 1.0 s and channel 1 over 0.5 s
   (`SndBgmCancelChannel`, `SndBgmQueueStop`). */

void BtlStopBgm(void)
{
    SndBgmCancelChannel(0);
    SndBgmQueueStop(1.0f, 0);
    SndBgmCancelChannel(1);
    SndBgmQueueStop(0.5f, 1);
}
