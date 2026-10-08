// bdc 0x08886260 BtlMotionDataSetTrackKeys
#include "bdc.h"

/* Overwrites every key of the tracks of motion `motion` whose channel id `paramD` is `'H'` and
   whose `ref` equals `target` with the 3-float vector `value`. A track's keys start at its `data`
   plus the byte offset `table[i]`; `paramA` is the key count. Tracks with bit 0x80 of `param8`
   hold 8-byte half keys (`GmoMotionKeyH`, values re-packed with `BtlFloatToHalf`), the
   others 16-byte float keys (`GmoMotionKeyF`). Used by `BtlMotionSetTrackKeysByName`. */
void BtlMotionDataSetTrackKeys(GmoMotionInfo *motion, u32 target, const float *value)
{
    const GmoMotionTrack *track = (const GmoMotionTrack *)PspPtr(motion->tracks);
    const u16 *offset = (const u16 *)PspPtr(motion->table);
    int i;

    for (i = 0; i < motion->trackCount; i++, track++, offset++) {
        u8 *keys;
        int k;

        if (track->paramD != 'H' || track->ref != target) {
            continue;
        }
        keys = (u8 *)PspPtr(track->data) + *offset;
        if ((track->param8 & 0x80) != 0) {
            GmoMotionKeyH *key = (GmoMotionKeyH *)keys;

            for (k = 0; k < track->paramA; k++, key++) {
                key->value[0] = BtlFloatToHalf(value[0]);
                key->value[1] = BtlFloatToHalf(value[1]);
                key->value[2] = BtlFloatToHalf(value[2]);
            }
        } else {
            GmoMotionKeyF *key = (GmoMotionKeyF *)keys;

            for (k = 0; k < track->paramA; k++, key++) {
                key->value[0] = value[0];
                key->value[1] = value[1];
                key->value[2] = value[2];
            }
        }
    }
}
