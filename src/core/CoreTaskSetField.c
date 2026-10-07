// bdc 0x089bf2d4 CoreTaskSetField
#include "bdc.h"

/* Virtual setter in slot 5 of the `CoreTask` vtable: stores `value` into the task's first three
   words selected by `index` (0 = `id`, 1 = `flags`, 2 = the child-task word at `+8`); any other
   index is ignored. */
void CoreTaskSetField(CoreTask *task, s32 index, u32 value)
{
    switch (index) {
    case 0:
        task->id = value;
        break;
    case 1:
        task->flags = value;
        break;
    case 2:
        task->field2 = value;
        break;
    default:
        break;
    }
}
