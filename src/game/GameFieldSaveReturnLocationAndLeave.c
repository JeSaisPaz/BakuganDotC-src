// bdc 0x088bf37c GameFieldSaveReturnLocationAndLeave
#include "bdc.h"

/* Backs up the location (`GameFieldSaveReturnLocation`) and sets the pending leave request
   `leaveRequest = 0xe` of the field task (id 500, `GameFieldCtor`). */

void GameFieldSaveReturnLocationAndLeave(CoreTask *task)

{
  GameFieldTask *field = (GameFieldTask *)task;

  GameFieldSaveReturnLocation();
  field->leaveRequest = 0xe;
}
