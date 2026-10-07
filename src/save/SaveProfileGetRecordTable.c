// bdc 0x0880cdb0 SaveProfileGetRecordTable
#include "bdc.h"

/* Returns the 0x1e0-byte record table at `record area + 4` (`SaveProfileGetRecordArea`), or NULL.
    */

void * SaveProfileGetRecordTable(SaveProfile *self)

{
  SaveRecordArea *area;
  u8 *table;
  
  table = (u8 *)0x0;
  area = (SaveRecordArea *)SaveProfileGetRecordArea(self);
  if (area != (SaveRecordArea *)0x0) {
    table = area->table;
  }
  return table;
}
