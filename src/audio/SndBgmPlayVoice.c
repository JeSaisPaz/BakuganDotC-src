// bdc 0x089c2f7c SndBgmPlayVoice
#include "bdc.h"

/* Plays a voice line (`VO_%d.at3`, track ids >= 10000) on streaming player 1, provided its file is
   already preloaded in CODataMng. If player 1 is idle (track id -1) the voice is started on it
   directly; otherwise a stop command (no fade) is queued for player 1. When the voice was not
   started directly, a play command for it is queued (`SndBgmQueuePlay`). Returns the result of
   `SndBgmPlayerPlayTrack` when it was called, else 0 (also 0 when the file is not preloaded or
   player 1 does not exist). */

s32 SndBgmPlayVoice(s32 voiceId)
{
  char *path;
  void *found;
  s32 result;

  result = 0;
  path = SndBuildVoiceFilePath(voiceId);
  if (path != NULL && IoDataMngExists()) {
    found = IoDataMngFindByPath(IoGetDataMng(), path);
    if (found != NULL && SndBgmPlayerExists(1)) {
      if (SndBgmPlayerGetTrackId(SndBgmPlayerGet(1)) == -1) {
        result = SndBgmPlayerPlayTrack(SndBgmPlayerGet(1), voiceId, 0, 0);
      } else {
        SndBgmQueueStop(0.0f, 1);
      }
      if (result == 0) {
        SndBgmQueuePlay(1, voiceId, 0, 0);
      }
    }
  }
  return result;
}
