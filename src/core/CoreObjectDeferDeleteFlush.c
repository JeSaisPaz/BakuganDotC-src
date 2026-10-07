// bdc 0x089e0fd4 CoreObjectDeferDeleteFlush
#include "bdc.h"

/* Flushes the deferred-deletion queue `g_coreObjectDeferDeleteList` (filled by `CoreObjectDeferDelete`): objects
   whose delay (`id` field) is 0 are deleted through their virtual destructor (flags 3); the others
   get their delay cleared so they go next time. Run every frame from `CoreTaskManagerUpdate` and
   at battle teardown (`BtlMainTaskDtor`, `BtlMainTeardown`). */
void CoreObjectDeferDeleteFlush(void)
{
    CoreObject *obj = g_coreObjectDeferDeleteList.head;
    CoreObject *next;

    while (obj != NULL) {
        next = obj->next;
        if (obj->id == 0) {
            /* virtual deleting destructor: vtable entry 1, flags 3 */
            const VtblEntry *dtor = &((const VtblEntry *)obj->vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
        }
        else {
            obj->id = 0;
        }
        obj = next;
    }
}
