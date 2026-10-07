// bdc 0x089c2b20 SndGetStreamFilePath
#include "bdc.h"

/* Maps a stream id to the file name of its ATRAC3 `.at3` file: ids 0..0x28 index the name table
   `g_soundStreamFileNames` (`sound/BGM_ADV_JPN.at3`, `sound/BGM_BTL_USA.at3`,
   `sound/BGM_TITLE.at3`, jingles `sound/JNG_WIN.at3`, ...); ids >= 10000 are voice lines and go
   through `SndBuildVoiceFilePath`; every other id (negative or 0x29..9999) returns NULL. */

char *SndGetStreamFilePath(s32 id)

{
  char *path;

  path = (char *)0x0;
  if (id < 10000) {
    if ((-1 < id) && ((uint)id < 0x29)) {
      return g_soundStreamFileNames[id];
    }
  }
  else {
    path = SndBuildVoiceFilePath(id);
  }
  return path;
}
