// bdc 0x089c8f94 SndEmitterGroupSelectNearest
#include "bdc.h"

/* Arbitrates exclusive emitter groups so that only the nearest emitter of each group sounds. It
   walks the global list of emitter lists `g_soundEmitterGroupList` (a list whose entries are
   emitter lists); for every emitter of every list it computes the squared distance from the
   listener position `listenerPos` to the emitter's `posA`..`posB` (`SndPointSegmentDistSq`),
   takes over and clears any live voice handle, and sets `silenced = 1`. Afterwards the nearest
   emitter of the list gets `silenced = 0` and inherits the last live voice handle found among its
   siblings. */

void SndEmitterGroupSelectNearest(const float *listenerPos)
{
    CorePrioNode *groupNode;
    CorePrioNode *node;
    CorePrioList *list;
    SndEmitter *emitter;
    SndEmitter *nearest;
    s32 liveHandle;
    s32 handle;
    float dist;
    float bestDist = 0.0f;

    if (g_soundEmitterGroupList == NULL) {
        return;
    }
    SndEmitterGroupListFlush(g_soundEmitterGroupList);
    groupNode = SndEmitterGroupNodeGetNext(SndEmitterGroupListHead(g_soundEmitterGroupList));
    while (groupNode != NULL) {
        list = SndEmitterGroupNodeGetData(groupNode);
        groupNode = SndEmitterGroupNodeGetNext(groupNode);
        if (list == NULL) {
            continue;
        }
        nearest = NULL;
        liveHandle = -1;
        SndEmitterListFlush(list);
        node = SndEmitterNodeGetNext(SndEmitterListHead(list));
        while (node != NULL) {
            emitter = SndEmitterNodeGetData(node);
            node = SndEmitterNodeGetNext(node);
            if (emitter == NULL) {
                break;
            }
            dist = SndPointSegmentDistSq(listenerPos, emitter->posA, emitter->posB, NULL);
            /* c.le.s + bc1tl: a NaN distance also takes over */
            if (nearest == NULL || !(bestDist <= dist)) {
                bestDist = dist;
                nearest = emitter;
            }
            handle = emitter->handle;
            if (handle >= 0) {
                liveHandle = handle;
                emitter->handle = -1;
            }
            emitter->silenced = 1;
        }
        if (nearest != NULL) {
            nearest->silenced = 0;
            nearest->handle = liveHandle;
        }
    }
}
