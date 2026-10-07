// bdc 0x08a2debc SndEmitterListRewind
#include "bdc.h"

/* Resets the iteration cursor (`list+8`) to the first node of the live chain (`head->next`).
   Iterate with `SndEmitterListNext`. */
void SndEmitterListRewind(CorePrioList *list)
{
    list->cursor = SndEmitterNodeGetNext(list->active);
}
