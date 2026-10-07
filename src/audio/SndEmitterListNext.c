// bdc 0x08a2dee8 SndEmitterListNext
#include "bdc.h"

/* Iterator step: returns the data of the node at the cursor and advances the cursor, skipping nodes
   flagged removed; returns NULL at the end of the live chain. */

SndEmitter *SndEmitterListNext(CorePrioList *list) {
    SndEmitter *result = NULL;

    while (list->cursor != NULL) {
        if (!SndEmitterNodeIsRemoved(list->cursor)) {
            result = SndEmitterNodeGetData(list->cursor);
        }
        list->cursor = SndEmitterNodeGetNext(list->cursor);
        if (result != NULL) {
            break;
        }
    }
    return result;
}
