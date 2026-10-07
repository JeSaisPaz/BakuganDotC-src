// bdc 0x0880e39c SaveFillArenaRecordSfo
#include "bdc.h"

/* Fills the PARAM.SFO strings of the arena-record save file: `"BAKUGAN2 PORTABLE"` into `title`,
   `"Arena Record"` at `sfo + 0x80` and `"Total play records in arena."` at `sfo + 0x100`. Called by
   `SysUtilSavedataHandlerRequest`. */

void SaveFillArenaRecordSfo(char *title, char *sfo)

{
  strcpy(title, g_arenaSfoStrings[0]);
  strcpy(sfo + 0x80, g_arenaSfoStrings[1]);
  strcpy(sfo + 0x100, g_arenaSfoStrings[2]);
}
