// bdc 0x088ea820 GameEventPropCreate
#include "bdc.h"

/* Allocates (low heap) and constructs an event prop (`GameEventPropCtor`) and creates its model
   (`GameEventPropCreateModel`); on failure releases and deletes it and returns NULL. */

void *GameEventPropCreate(s16 kind)
{
    bool fromLow;
    void *mem;
    void *prop;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GameEventProp), (char *)0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    prop = (void *)0;
    if (mem != (void *)0) {
        GameEventPropCtor(mem);
        prop = mem;
    }
    if (GameEventPropCreateModel(prop, kind) == 0) {
        GameEventPropReleaseModel(prop);
        GameEventPropDelete(prop);
        prop = (void *)0;
    }
    return prop;
}
