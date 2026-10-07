// bdc 0x089c079c SndEmitterAssignGroups
#include "bdc.h"

/* Groups the emitter list by sound id for volume sharing: it flushes `g_soundEmitterList`, walks
   it in priority order and, whenever the sound id changes (or the list ends), closes the previous
   group by writing the member count (`groupCount`) and the sum of the members' `targetVolume`
   (`groupVolSum`) into every emitter collected in `g_soundEmitterScratchList` and clearing their
   `silenced` flag, then empties the scratch list. Only point emitters (`params->shape == 1`) that
   are not released and whose sound id lies in one of the ranges of `g_soundGroupRanges`
   (`{lo, hi, ?}` rows, row count `g_soundGroupRangeCount`) are collected, keyed by their squared
   distance from the listener (`SndListenerDistSq`, or `0x7fffffff` for the +infinity sentinel);
   emitters that are playing (`state38 >= 0`) contribute to the count and volume sum. Returns
   early, doing nothing, when either list is missing or the emitter list is empty. */

void SndEmitterAssignGroups(SndListener *listener)
{
    CoreList unused; /* constructed and destroyed, never used */
    SndEmitter *emitter;
    SndEmitter *member;
    s32 *row;
    s32 groupId;
    s32 count;
    s32 rangeCount;
    s32 i;
    s32 priority;
    s32 closeGroup;
    s32 running;
    float volSum;
    float dist;

    volSum = 0.0f;
    NetAdhocListInit(&unused, 0);
    running = 1;
    if (g_soundEmitterScratchList != NULL && g_soundEmitterList != NULL) {
        SndEmitterListFlush(g_soundEmitterList);
        SndEmitterListRewind(g_soundEmitterList);
        groupId = -1;
        count = 0;
        if (SndEmitterListHasNodes(g_soundEmitterList)) {
            do {
                closeGroup = 0;
                emitter = SndEmitterListNext(g_soundEmitterList);
                if (emitter == NULL) {
                    closeGroup = 1;
                } else if (groupId != emitter->soundId) {
                    closeGroup = 1;
                }
                if (closeGroup) {
                    SndEmitterListFlush(g_soundEmitterScratchList);
                    if (SndEmitterListHasNodes(g_soundEmitterScratchList)) {
                        while ((member = SndEmitterListNext(g_soundEmitterScratchList)) != NULL) {
                            member->silenced = 0;
                            member->groupCount = count;
                            member->groupVolSum = volSum;
                        }
                    }
                    SndEmitterListClear(g_soundEmitterScratchList);
                    groupId = -1;
                    volSum = 0.0f;
                    count = 0;
                    if (emitter == NULL) {
                        running = 0;
                    } else {
                        rangeCount = g_soundGroupRangeCount;
                        row = g_soundGroupRanges;
                        for (i = 0; i < rangeCount; i++, row += 3) {
                            if (row[0] <= emitter->soundId && emitter->soundId <= row[1]) {
                                groupId = emitter->soundId;
                                break;
                            }
                        }
                    }
                }
                if (groupId >= 0 && emitter->soundId == groupId && emitter->params->shape == 1 &&
                    !emitter->released) {
                    if (emitter->state38 >= 0) {
                        count++;
                        volSum += emitter->targetVolume;
                    }
                    if (emitter->handle == -1) {
                        emitter->silenced = 1;
                    }
                    if (emitter->src != NULL) {
                        emitter->posA[0] = emitter->src[0];
                        emitter->posA[1] = emitter->src[1];
                        emitter->posA[2] = emitter->src[2];
                    }
                    dist = SndListenerDistSq(listener, emitter->posA);
                    priority = 0x7fffffff;
                    if (dist != g_floatPosInf) {
                        priority = (s32)dist;
                    }
                    SndEmitterListInsert(g_soundEmitterScratchList, emitter, priority);
                }
            } while (running);
        }
    }
    NetAdhocListDestroy(&unused, 2);
}
