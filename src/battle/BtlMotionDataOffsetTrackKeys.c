// bdc 0x08885e90 BtlMotionDataOffsetTrackKeys
#include "bdc.h"

/* Adds the 3-float vector `offset` to every key of the tracks of motion `motion` whose channel id
   `paramD` is `'H'` and whose `ref` equals `target`. A track's keys start at its `data` plus the
   byte offset `table[i]`; `paramA` is the key count. Tracks with bit 0x80 of `param8` hold 8-byte
   half keys (`GmoMotionKeyH`: unpacked as with VFPU `vh2f`, offset, re-packed with
   `BtlFloatToHalf`), the others 16-byte float keys (`GmoMotionKeyF`, edited in place). Used by
   `BtlMotionAddTrackOffsetByName`. */
void BtlMotionDataOffsetTrackKeys(GmoMotionInfo *motion, u32 target, float *offset)
{
    const GmoMotionTrack *track = motion->tracks;
    const u16 *keyOffset = motion->table;
    int i;

    for (i = 0; i < motion->trackCount; i++, track++, keyOffset++) {
        u8 *keys;
        int k;

        if (track->paramD != 'H' || track->ref != target) {
            continue;
        }
        keys = (u8 *)track->data + *keyOffset;
        if ((track->param8 & 0x80) != 0) {
            GmoMotionKeyH *key = (GmoMotionKeyH *)keys;

            for (k = 0; k < track->paramA; k++, key++) {
                float x = VfH2f(key->value[0]) + offset[0];
                float y = VfH2f(key->value[1]) + offset[1];
                float z = VfH2f(key->value[2]) + offset[2];

                key->value[0] = BtlFloatToHalf(x);
                key->value[1] = BtlFloatToHalf(y);
                key->value[2] = BtlFloatToHalf(z);
            }
        } else {
            GmoMotionKeyF *key = (GmoMotionKeyF *)keys;

            for (k = 0; k < track->paramA; k++, key++) {
                float x = key->value[0] + offset[0];
                float y = key->value[1] + offset[1];
                float z = key->value[2] + offset[2];

                key->value[0] = x;
                key->value[1] = y;
                key->value[2] = z;
            }
        }
    }
}
