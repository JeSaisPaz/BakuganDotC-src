// bdc 0x08a2961c CoreTaskBaseDraw
#include "bdc.h"

/* Default (empty) draw method of the `CoreTask` base class: vtable slot 4 (`vt+0x24`), called
   every frame by `CoreTaskManagerDraw`. */
void CoreTaskBaseDraw(CoreTask *task)
{
    (void)task;
}
