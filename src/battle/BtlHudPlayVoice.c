// bdc 0x0883ac9c BtlHudPlayVoice
#include "bdc.h"

/* Plays advisor voice `voiceId` on BGM player 0: formats `"VO_%d.at3"` into `voiceName` and, only
   when the data manager exists, has a live request for that file (`IoDataMngFindByPath`) and BGM
   player 0 exists, either starts the track directly (`SndBgmPlayerPlayTrack`, when the player
   has no current track) or queues a stop with no fade (`SndBgmQueueStop`). If the track was not
   started directly (stop queued, or the direct start refused) it queues the play instead
   (`SndBgmQueuePlay`). Returns the direct start's result (0 when it was not started directly). */

int BtlHudPlayVoice(BtlHud *self, int voiceId)
{
    int started = 0;

    sprintf(self->voiceName, "VO_%d.at3", voiceId);
    if (IoDataMngExists() && IoDataMngFindByPath(IoGetDataMng(), self->voiceName) != NULL &&
        SndBgmPlayerExists(0)) {
        if (SndBgmPlayerGetTrackId(SndBgmPlayerGet(0)) == -1) {
            started = SndBgmPlayerPlayTrack(SndBgmPlayerGet(0), voiceId, 0, 0);
        } else {
            SndBgmQueueStop(0.0f, 0);
        }
        if (started == 0) {
            SndBgmQueuePlay(0, voiceId, 0, 0);
        }
    }
    return started;
}
