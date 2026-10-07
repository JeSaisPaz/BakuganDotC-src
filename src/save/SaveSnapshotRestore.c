// bdc 0x088c2aa0 SaveSnapshotRestore
#include "bdc.h"

/* When a snapshot exists, restores the save data block from it (discarding unsaved progress) while
   keeping the four option bytes `+0x6a8..+0x6ab` of the profile (re-applied with `SaveProfileSetBgmVolume`,
   `SaveProfileSetSeVolume`, `SaveProfileSetVoiceVolume`), then clears the snapshot (`SaveSnapshotClear`). */

void SaveSnapshotRestore(void)

{
  u8 bgm;
  u8 se;
  u8 voice;
  u8 advice;
  void *dst;
  size_t n;

  if (g_saveSnapshotEnabled != '\0') {
    bgm = SaveGetProfile()->data->bgmVolume;
    se = SaveGetProfile()->data->seVolume;
    voice = SaveGetProfile()->data->voiceVolume;
    advice = SaveGetProfile()->data->adviceOff;
    dst = SaveGetDataBlock();
    n = SaveGetDataBlockSize();
    memcpy(dst,g_saveSnapshotBuffer,n);
    SaveProfileSetBgmVolume(SaveGetProfile(),bgm);
    SaveProfileSetSeVolume(SaveGetProfile(),se);
    SaveProfileSetVoiceVolume(SaveGetProfile(),voice);
    SaveGetProfile()->data->adviceOff = advice;
    SaveSnapshotClear();
  }
  return;
}
