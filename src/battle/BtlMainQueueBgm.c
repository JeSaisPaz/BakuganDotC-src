// bdc 0x0884e068 BtlMainQueueBgm
#include "bdc.h"

/* Changes the battle background music: when `bgmState` is zero, queues a stop of BGM channel 0
   with fade-out `fadeSeconds` (`SndBgmQueueStop`) and then starts BGM `bgmId` on channel 0
   (`SndBgmQueuePlay``(0, bgmId, 1, 0)`); in either case remembers `bgmId` in `bgmOverride`. */
void BtlMainQueueBgm(BtlMain *self, s32 bgmId, float fadeSeconds)
{
    if (self->bgmState == 0) {
        SndBgmQueueStop(fadeSeconds, 0);
        SndBgmQueuePlay(0, bgmId, 1, 0);
    }
    self->bgmOverride = bgmId;
}
