// bdc 0x088bf3a0 GameFieldEmptyHookA
#include "bdc.h"

/* Empty hook taking a task pointer and doing nothing (called by `GameFieldUnloadArea` and
   `GameEventOpAFClearRoomSetting`). */
void GameFieldEmptyHookA(CoreTask *task)
{
    (void)task;
}
