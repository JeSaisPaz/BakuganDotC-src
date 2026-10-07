// bdc 0x089c0544 SndEmitterDestroyAll
#include "bdc.h"

/* Scene-teardown path of the positional sound system: first runs the shutdown of the
   group-loader/exclusive-group side (`SndReleaseAll`, which disables the `SndGroupLoader` and
   marks all its requests released), then, if `g_soundEmitterList` exists, flushes it and calls
   `SndEmitterDestroy` on the data of every node of both the committed chain
   (`SndEmitterListHead`) and the pending chain (`SndEmitterListPendingHead`), skipping removed
   nodes and NULL data, and flushes once more. */
void SndEmitterDestroyAll(SndListener *listener)
{
    CorePrioNode *node;
    SndEmitter *emitter;

    SndReleaseAll();
    if (g_soundEmitterList == NULL) {
        return;
    }
    SndEmitterListFlush(g_soundEmitterList);

    for (node = SndEmitterNodeGetNext(SndEmitterListHead(g_soundEmitterList)); node != NULL;
         node = SndEmitterNodeGetNext(node)) {
        emitter = SndEmitterNodeGetData(node);
        if (!SndEmitterNodeIsRemoved(node) && emitter != NULL) {
            SndEmitterDestroy(listener, emitter);
        }
    }

    for (node = SndEmitterNodeGetNext(SndEmitterListPendingHead(g_soundEmitterList)); node != NULL;
         node = SndEmitterNodeGetNext(node)) {
        emitter = SndEmitterNodeGetData(node);
        if (!SndEmitterNodeIsRemoved(node) && emitter != NULL) {
            SndEmitterDestroy(listener, emitter);
        }
    }

    SndEmitterListFlush(g_soundEmitterList);
}
