// bdc 0x0890798c BtlDemoScbEventGroupReadEvents
#include "bdc.h"

/* Allocates one low-heap event node per record of a `.scb` event group (record count `data[0]`,
   records packed after the 12-byte header), as the event class of the group type `type`
   (from `BtlDemoScbEventGroupType`): 1 key-track (`BtlDemoScbKeyTrackEventCtor`, 0x34 bytes),
   2 cue (`BtlDemoScbCueEventCtor`, 0x28), 3 motion (`BtlDemoScbMotionEventCtor`, 0x3c),
   4 pose (`BtlDemoScbPoseEventCtor`, 0x2c), 5 sound (`BtlDemoScbSoundEventCtor`, 0x34),
   6 cue B (`BtlDemoScbCueEventBCtor`, 0x28), each linked into the group's list for that type.
   A node then reads its record (`BtlDemoScbEventRead`, which returns the record size) and gets
   `groupType = type` and `groupWord` = the header word at byte 4. A record without a node (types
   7, 8 and anything outside 1..8, or a failed allocation) is skipped as 4 bytes. */
void BtlDemoScbEventGroupReadEvents(BtlDemoScbEventGroup *group, u16 *data, s32 type)
{
    u16 count = data[0];
    const u8 *records = (const u8 *)&data[6];
    s32 offset = 0;
    u32 i;
    bool wasLow;
    BtlDemoScbEvent *ev = NULL;

    for (i = 0; i < count; i++) {
        u16 *rec = (u16 *)(records + offset);

        switch (type) {
        case 1:
            MemLock();
            wasLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            ev = MemAlloc(0x34, NULL, 0);
            MemSetAllocFromLow(wasLow);
            MemUnlock();
            if (ev != NULL) {
                BtlDemoScbKeyTrackEventCtor(ev, group->lists[0]);
            }
            break;
        case 2:
            MemLock();
            wasLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            ev = MemAlloc(0x28, NULL, 0);
            MemSetAllocFromLow(wasLow);
            MemUnlock();
            if (ev != NULL) {
                BtlDemoScbCueEventCtor(ev, group->lists[1]);
            }
            break;
        case 3:
            MemLock();
            wasLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            ev = MemAlloc(0x3c, NULL, 0);
            MemSetAllocFromLow(wasLow);
            MemUnlock();
            if (ev != NULL) {
                BtlDemoScbMotionEventCtor((BtlDemoScbMotionEvent *)ev, group->lists[2]);
            }
            break;
        case 4:
            MemLock();
            wasLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            ev = MemAlloc(0x2c, NULL, 0);
            MemSetAllocFromLow(wasLow);
            MemUnlock();
            if (ev != NULL) {
                BtlDemoScbPoseEventCtor(ev, group->lists[3]);
            }
            break;
        case 5:
            MemLock();
            wasLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            ev = MemAlloc(0x34, NULL, 0);
            MemSetAllocFromLow(wasLow);
            MemUnlock();
            if (ev != NULL) {
                BtlDemoScbSoundEventCtor((BtlDemoScbSoundEvent *)ev, group->lists[4]);
            }
            break;
        case 6:
            MemLock();
            wasLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            ev = MemAlloc(0x28, NULL, 0);
            MemSetAllocFromLow(wasLow);
            MemUnlock();
            if (ev != NULL) {
                BtlDemoScbCueEventBCtor(ev, group->lists[5]);
            }
            break;
        default:
            /* `ev` keeps its previous value; `type` is the same on every pass, so that is NULL. */
            break;
        }
        if (ev != NULL) {
            offset += BtlDemoScbEventRead(ev, rec);
            ev->groupType = type;
            ev->groupWord = *(const u32 *)&data[2];
        } else {
            offset += 4;
        }
    }
}
