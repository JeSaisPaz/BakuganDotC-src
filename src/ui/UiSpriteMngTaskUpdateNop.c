// bdc 0x089ee424 UiSpriteMngTaskUpdateNop
#include "bdc.h"

/* Empty update method (vtable `0x08af57ac` slot 2) of the 2D sprite manager task
   (`UiSpriteMngEnsureTask`); all its work happens in `UiSpriteMngTaskDraw`. */
void UiSpriteMngTaskUpdateNop(CoreTask *task)
{
    (void)task;
}
