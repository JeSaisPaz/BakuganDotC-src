// bdc 0x089c59bc SndBuildGroupPath
#include "bdc.h"

/* Builds the file path of sound group `groupId` into `out`: `"sound/"`, then for the
   language-specific groups 0x43..0x52 a per-language directory name (`g_soundLangDirNames`, indexed by
   the profile's language from `SaveProfileGetLanguage`, or 1 when there is no profile) and `"/"`,
   then the group's file name from the 0x53-entry `g_soundGroupNames` (`seGRP_SYS_COM.pac` is
   entry 0). */

void SndBuildGroupPath(char *out, s32 groupId)

{
  bool langDir;
  SaveProfile *self;
  s32 lang;

  langDir = false;
  switch(groupId) {
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x4a:
  case 0x4b:
  case 0x4c:
  case 0x4d:
  case 0x4e:
  case 0x4f:
  case 0x50:
  case 0x51:
  case 0x52:
    langDir = true;
  }
  strcpy(out,"sound/");
  if (langDir) {
    lang = 1;
    if (SaveHasProfile()) {
      self = SaveGetProfile();
      lang = SaveProfileGetLanguage(self);
    }
    strcat(out,g_soundLangDirNames[lang]);
    strcat(out,"/");
  }
  strcat(out,g_soundGroupNames[groupId]);
  return;
}

