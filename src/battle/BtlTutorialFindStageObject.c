// bdc 0x088469e0 BtlTutorialFindStageObject
#include "bdc.h"

/* Returns the first `StopWall` in `g_stopWallList` whose wall `id` equals `id`, or NULL (also
   when the list does not exist yet). `task` is unused. */
void *BtlTutorialFindStageObject(void *task, int id)
{
    CoreObject *obj;

    (void)task;
    if (g_stopWallList == NULL) {
        return NULL;
    }
    for (obj = g_stopWallList->head; obj != NULL; obj = obj->next) {
        if (((StopWall *)obj)->id == id) {
            return obj;
        }
    }
    return NULL;
}
