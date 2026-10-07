// bdc 0x0880a410 SaveNopTaskCtor
#include "bdc.h"

/* Constructor of the empty save task id 10011 (0x271b, 0x18 bytes, vtable `0x08af153c`): only
   `CoreTaskInit` and a cleared word at `+0x10`. Its update (`SaveNopTaskUpdate`) removes the
   task on the first frame, so the id is a no-op placeholder in the 10009–10030 save-task range
   (between 10010 load and 10020 save). */

SaveNopTask *SaveNopTaskCtor(SaveNopTask *task)

{
  CoreTaskInit(&task->base);
  task->base.vtable = &g_saveNopTaskVtbl;
  task->word10 = 0;
  return task;
}

