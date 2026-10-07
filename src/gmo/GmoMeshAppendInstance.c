// bdc 0x08a15cc0 GmoMeshAppendInstance
#include "bdc.h"

/* Appends an instance at the end of a mesh entry's instance list (`+4`). */

void GmoMeshAppendInstance(void *mesh, void *inst)
{
    GmoInstance *link = (GmoInstance *)mesh;
    GmoInstance *next;

    if (link != NULL && inst != NULL) {
        next = link->next;
        while (next != NULL) {
            link = next;
            next = link->next;
        }
        link->next = (GmoInstance *)inst;
    }
}
