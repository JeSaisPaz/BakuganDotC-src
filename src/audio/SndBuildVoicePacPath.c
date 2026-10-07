// bdc 0x089c28c4 SndBuildVoicePacPath
#include "bdc.h"

/* Builds the path of the voice package `index` in the static 0x80-byte buffer `g_voicePacPathBuf` and
   returns it: `"voice/"` + the language code (`SaveProfileGetLanguage`, from `g_soundLangDirNames`
   as in `SndBuildVoiceFilePath`, index 1 without a profile) + `"/"` + the name
   `g_soundVoicePacNames[index]` (e.g. `VO_PAC_BATTLE_00`, `VO_PAC_PMF_UK_1_10`,
   `VO_PAC_EXPLORATION_03`) + `".pac"`. The pieces are joined with `strcpy`/`strcat` in place. */

char *SndBuildVoicePacPath(s32 index)
{
  SaveProfile *profile;
  s32 lang;

  lang = 1;
  if (SaveHasProfile()) {
    profile = SaveGetProfile();
    lang = SaveProfileGetLanguage(profile);
  }
  strcpy(g_voicePacPathBuf, "voice/");
  strcat(g_voicePacPathBuf, g_soundLangDirNames[lang]);
  strcat(g_voicePacPathBuf, "/");
  strcat(g_voicePacPathBuf, g_soundVoicePacNames[index]);
  strcat(g_voicePacPathBuf, ".pac");
  return g_voicePacPathBuf;
}
