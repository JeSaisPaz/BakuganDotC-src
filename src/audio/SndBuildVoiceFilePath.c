// bdc 0x089c2808 SndBuildVoiceFilePath
#include "bdc.h"

/* Builds the file name of voice line `voiceId` in the static 0x80-byte buffer `g_voiceFilePathBuf` and
   returns it. Ids `0x2a3f..0x2a7a` (60 lines) are language specific and become
   `voice/<LANG>/VO_<id>.at3` (`sprintf` with `"voice/%s/VO_%d.at3"`), where `<LANG>` is the
   two-letter code at index `SaveProfileGetLanguage``(profile)` of `g_soundLangDirNames` (1
   `EN`, 2 `FR`, 3 `ES`, 4 `DE`, 5 `IT`, 6 `NE`; index 1 is used when there is no save profile yet,
   `SaveHasProfile`); every other id becomes the plain `VO_<id>.at3` (`"VO_%d.at3"`). The text is
   built on the stack and copied to the static buffer, so the pointer stays valid until the next
   call. */

char *SndBuildVoiceFilePath(s32 voiceId)
{
  SaveProfile *profile;
  s32 lang;
  char buf[64];

  if ((voiceId < 0x2a3f) || (0x2a7a < voiceId)) {
    sprintf(buf, "VO_%d.at3", voiceId);
  }
  else {
    lang = 1;
    if (SaveHasProfile()) {
      profile = SaveGetProfile();
      lang = SaveProfileGetLanguage(profile);
    }
    sprintf(buf, "voice/%s/VO_%d.at3", g_soundLangDirNames[lang], voiceId);
  }
  strcpy(g_voiceFilePathBuf, buf);
  return g_voiceFilePathBuf;
}
