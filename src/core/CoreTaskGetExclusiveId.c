// bdc 0x089bfa94 CoreTaskGetExclusiveId
#include "bdc.h"

/* Returns the exclusive task id (`g_taskExclusiveId`, set by `CoreTaskSetExclusiveId`): the
   id of the modal task that alone may update (`CoreTaskIsIdAllowed`). */
u32 CoreTaskGetExclusiveId(void)
{
    return g_taskExclusiveId;
}
