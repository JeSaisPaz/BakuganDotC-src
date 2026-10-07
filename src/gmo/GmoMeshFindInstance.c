// bdc 0x08a15c84 GmoMeshFindInstance
#include "bdc.h"

/* Returns the `n`-th instance in a mesh entry's instance list (`+4`, `next` at `+4`) whose id
   (`+0xe`) equals `id`, or NULL. */

void *GmoMeshFindInstance(void *mesh, u32 id, int n)
{
    GmoInstance *inst;

    if (mesh != NULL) {
        inst = ((GmoInstance *)mesh)->next;
        while (inst != NULL) {
            if (inst->id == id) {
                n--;
                if (n < 0) {
                    return inst;
                }
            }
            inst = inst->next;
        }
    }
    return NULL;
}
