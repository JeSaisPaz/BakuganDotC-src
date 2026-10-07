// bdc 0x089bf314 CoreTaskGetField
#include "bdc.h"

/* Virtual getter in slot 6 of the `CoreTask` vtable: returns the task word selected by `index` (0
   = `id`, 1 = `flags`, 2 = the child-task word at `+8`), 0 for any other index. */
u32 CoreTaskGetField(CoreTask *task, s32 index)
{
    switch (index) {
    case 0:
        return task->id;
    case 1:
        return task->flags;
    case 2:
        return task->field2;
    default:
        return 0;
    }
}
