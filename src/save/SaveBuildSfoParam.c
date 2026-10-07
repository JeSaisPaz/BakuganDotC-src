// bdc 0x0880e084 SaveBuildSfoParam
#include "bdc.h"

/* Fills the savedata `PARAM.SFO` texts for the PSP savedata utility (called by
   `SysUtilSavedataHandlerCtor`): when a profile exists, repoints `g_gameSfoText` to the
   `g_gameSfoStrings` row of the profile language (`SaveProfileGetLanguage`; languages 7..11 and
   out-of-range values keep the current row), copies the game title to `sfo->title`, formats
   `sfo->savedataTitle` with the player name (`SaveProfileGetPlayerName`, converted by
   `UiTextDecodeToUtf8`) and `sfo->detail` with the play time (counter 0 split into h:m:s,
   `SaveProfileGetCounter`) and the number of Bakugan owned (bits 1..20 of `ownedBakugan`). */

void SaveBuildSfoParam(PspUtilitySavedataSFOParam *sfo)
{
  char name[28];
  char nameUtf8[40];
  s32 playTime;
  s32 hours;
  s32 minutes;
  s32 seconds;
  s32 owned;
  s32 i;
  SaveProfile *profile;

  memset(name, 0, 0x19);
  memset(nameUtf8, 0, 0x27);
  if (SaveHasProfile()) {
    if (SaveProfileGetPlayerName(SaveGetProfile(), name) != 0) {
      UiTextDecodeToUtf8(nameUtf8, name);
    }
  }
  if (SaveHasProfile()) {
    switch (SaveProfileGetLanguage(SaveGetProfile())) {
    case 1:
      g_gameSfoText = g_gameSfoStrings[0];
      break;
    case 2:
      g_gameSfoText = g_gameSfoStrings[1];
      break;
    case 3:
      g_gameSfoText = g_gameSfoStrings[2];
      break;
    case 4:
      g_gameSfoText = g_gameSfoStrings[3];
      break;
    case 5:
      g_gameSfoText = g_gameSfoStrings[4];
      break;
    case 6:
      g_gameSfoText = g_gameSfoStrings[5];
      break;
    case 12:
      g_gameSfoText = g_gameSfoStrings[6];
      break;
    default:
      break;
    }
  }
  strcpy(sfo->title, g_gameSfoText[0]);
  sprintf(sfo->savedataTitle, g_gameSfoText[1], nameUtf8);

  owned = 0;
  playTime = SaveProfileGetCounter(SaveGetProfile(), 0);
  hours = playTime / 3600;
  minutes = (playTime - hours * 3600) / 60;
  seconds = (playTime - hours * 3600) % 60;
  i = 0;
  do {
    profile = SaveGetProfile();
    i++;
    if ((profile->data->ownedBakugan[i / 8] & (1 << (i % 8))) != 0) {
      owned++;
    }
  } while (i < 20);
  sprintf(sfo->detail, g_gameSfoText[2], hours, minutes, seconds, owned);
}
