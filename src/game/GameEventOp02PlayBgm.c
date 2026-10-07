// bdc 0x088ebc0c GameEventOp02PlayBgm
#include "bdc.h"

/* Handler of event opcode 0x02 (`GameEventExecCommand`): cancels channel 0 and stops the BGM with
   a 0.4 s fade; argument 0 selects track 3 and 0x20 track 0x22 (others: silence), which is queued
   at once (`SndBgmQueuePlay`) or, in skip mode, remembered in `+0x271` for the end of the event.
    */

void GameEventOp02PlayBgm(GameEvent *self, u8 flag, s16 arg) {
    u16 track = 0;

    if (flag == 0) {
        track = 3;
    } else if (flag == 0x20) {
        track = 0x22;
    }
    if ((self->flags & 1) != 0) {
        SndBgmCancelChannel(0);
        SndBgmQueueStop(0.4f, 0);
        self->bgmTrack = (u8)track;
        return;
    }
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.4f, 0);
    if (track != 0) {
        SndBgmQueuePlay(0, track, 1, 0);
    }
}
