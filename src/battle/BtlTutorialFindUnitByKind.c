// bdc 0x088468fc BtlTutorialFindUnitByKind
#include "bdc.h"

/* Returns the first unit in the battle's Bakugan chain (`BtlGetBakuganList`) whose virtual
   predicate (vtable entry 18, `+0x90`) returns non-zero and whose kind (object word `+8`) is
   `kind`; NULL when the list is missing or none matches. `task` is unused. */
void *BtlTutorialFindUnitByKind(void *task, int kind)
{
    CoreObjectList *list;
    CoreObject *obj;

    (void)task;
    list = BtlGetBakuganList();
    if (list == NULL) {
        return NULL;
    }
    for (obj = list->head; obj != NULL; obj = obj->next) {
        const VtblEntry *pred = &((const VtblEntry *)obj->vtable)[18];

        if (((s32 (*)(void *))pred->fn)((u8 *)obj + pred->delta) != 0 && (s32)obj->unk08 == kind) {
            return obj;
        }
    }
    return NULL;
}
