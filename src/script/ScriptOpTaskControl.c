// bdc 0x089cb654 ScriptOpTaskControl
#include "bdc.h"

/* Task control by id: reads a sub-command, a task id and a third operand, then acts on the task
   with that id in the global task list (`g_taskList`): 0 create (priority = operand, 100 if <=
   0), 1 destroy, 2 wait while it exists, 3/6 read the 'update disabled' / 'draw hidden' bit into a
   variable, 4/5 set/clear the update-disabled bit (`CoreTask` flag 1), 7/8 set/clear the
   draw-hidden bit (flag 2). Returns 2 for sub 2 while the task exists, else 0. */

int ScriptOpTaskControl(Script *script)
{
    u32 sub;
    u32 id;
    CoreTask *task;
    u32 priority;
    u32 *var;
    int result = 0;

    sub = ScriptReadU16(script);
    id = ScriptReadU16(script);
    var = NULL;
    priority = 0xffffffff;
    if (sub == 0) {
        priority = ScriptReadU16(script);
    } else {
        var = ScriptReadRef(script, 2);
    }
    task = (CoreTask *)CoreTaskFind(id);
    switch (sub) {
    case 0:
        if ((s32)priority < 1) {
            CoreTaskCreate(id, 100);
        } else {
            CoreTaskCreate(id, priority);
        }
        break;
    case 1:
        CoreTaskRemoveById(id);
        break;
    case 2:
        if (CoreTaskExists(id) == 1) {
            result = 2;
        }
        break;
    case 3:
        *var = 0;
        if (task != NULL && CoreTaskHasFlags(task, 1)) {
            *var = 1;
        }
        break;
    case 4:
        if (task != NULL) {
            CoreTaskSetFlags(task, 1);
        }
        break;
    case 5:
        if (task != NULL) {
            CoreTaskClearFlags(task, 1);
        }
        break;
    case 6:
        *var = 0;
        if (task != NULL && CoreTaskHasFlags(task, 2)) {
            *var = 1;
        }
        break;
    case 7:
        if (task != NULL) {
            CoreTaskSetFlags(task, 2);
        }
        break;
    case 8:
        if (task != NULL) {
            CoreTaskClearFlags(task, 2);
        }
        break;
    }
    return result;
}
