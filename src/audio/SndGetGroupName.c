// bdc 0x089c6db0 SndGetGroupName
#include "bdc.h"

/* Returns the file name of sound-effect group `groupId` from the 0x54-entry table of `seGRP_*`
   names at `0x08ac56e8` (`"seGRP_SYS_COM.pac"`, `"seGRP_SYS_STORY.pac"`, `"seGRP_BTL_COM.pac"`,
   `"seGRP_GTE_FIRE.pac"`, ...). `groupId` is clamped to `0..0x53` first. The first argument is not
   used. */

char *SndGetGroupName(void *unused, s32 groupId)

{
  if (groupId < 0) {
    groupId = 0;
  }
  else if (0x53 < groupId) {
    groupId = 0x53;
  }
  return g_soundGroupNames[groupId];
}

