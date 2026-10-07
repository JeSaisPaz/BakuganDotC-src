// bdc 0x089087a0 BtlDemoScbParse
#include "bdc.h"

/* Parses the key chunks of a `.scb` battle demo script into the scene `scene` (`BtlDemoScene`).
   With `raw == 0`, `data` is a file whose word 0 is a word offset and word 1 the chunk area size:
   parsing starts at word `1 + data[0]` (nothing happens if that address is NULL) and `size` is
   replaced by word 1; with `raw != 0`, `data` is the chunk area itself and `size` is its byte size.
   The chunk area holds an s16 chunk count at byte 4, a table header at byte `(count + 1) * 4 + 8`
   whose high halfword is a pair count, that many 4-byte pairs (copied into a temporary low-heap
   array that is freed again without being read), then up to `count` 12-byte chunk headers, each
   stopping the walk once its byte offset is >= `size`. Byte 5 of a header is the chunk type, its
   high halfword the record count `n`; the `n` records follow the header. Type 0 fills the next
   `track0` slot (0x34-byte records; also stores the 1-based type-0 ordinal) and types 1..5 the
   next slot of `tracks[type - 1]` (0x38, 0x30, 0x38, 0x30, 0x10-byte records). Each sets the
   slot's `keyCount` to `n`; for `n != 0` (type 0: also when `keys` is NULL) it frees the old
   `keys` array and allocates `n` records from the low heap (`MemSetAllocFromLow`), then copies
   the records. Types >= 6 consume only their header. */
void BtlDemoScbParse(u32 *data, void *scene, u32 size, s8 raw)
{
    static const u32 recordWords[5] = { 0x38 / 4, 0x30 / 4, 0x38 / 4, 0x30 / 4, 0x10 / 4 };
    BtlDemoScene *self = (BtlDemoScene *)scene;
    s32 trackSlot[5] = { 0, 0, 0, 0, 0 };
    s32 slot0 = 0;
    u32 ordinal0 = 0;
    u32 *pairs = NULL;
    u32 pairCount;
    u32 pos;
    s32 i;
    s32 j;
    bool wasLow;

    if (raw == 0) {
        size = data[1];
        data = data + 1 + data[0];
        if (data == NULL) {
            return;
        }
    }

    pos = (u32)((s16)data[1] + 1) + 2;
    pairCount = data[pos] >> 16;
    pos++;
    if (pairCount != 0) {
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        pairs = MemAlloc(pairCount * 4, NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        for (j = 0; j < (s32)pairCount; j++) {
            pairs[j] = data[pos];
            pos++;
        }
    }

    for (i = 0; i < (s16)data[1]; i++) {
        u32 header;
        u32 type;
        u32 count;
        u32 words;
        u32 *keyCount;
        void **keys;
        u32 *dst;

        if (pos * 4 >= size) {
            break;
        }
        header = data[pos + 1];
        pos += 3;
        type = (header >> 8) & 0xff;
        count = header >> 16;
        if (type >= 6) {
            continue;
        }
        if (type == 0) {
            keyCount = &self->track0[slot0].keyCount;
            keys = &self->track0[slot0].keys;
            words = 0x34 / 4;
            *keyCount = count;
            ordinal0++;
            self->track0[slot0].ordinal = ordinal0;
            slot0++;
            if (count == 0 && *keys != NULL) {
                continue;
            }
        } else {
            keyCount = &self->tracks[type - 1][trackSlot[type - 1]].keyCount;
            keys = &self->tracks[type - 1][trackSlot[type - 1]].keys;
            words = recordWords[type - 1];
            trackSlot[type - 1]++;
            *keyCount = count;
            if (count == 0) {
                continue;
            }
        }
        if (*keys != NULL) {
            MemLock();
            MemFree(*keys, NULL, 0);
            MemUnlock();
            *keys = NULL;
        }
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        *keys = MemAlloc(count * words * 4, NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        dst = (u32 *)*keys;
        for (j = 0; j < (s32)count; j++) {
            memcpy(&dst[j * words], &data[pos], words * 4);
            pos += words;
        }
    }

    if (pairs != NULL) {
        MemLock();
        MemFree(pairs, NULL, 0);
        MemUnlock();
    }
}
