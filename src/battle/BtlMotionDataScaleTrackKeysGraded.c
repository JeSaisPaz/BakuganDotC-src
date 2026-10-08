// bdc 0x08886024 BtlMotionDataScaleTrackKeysGraded
#include "bdc.h"

/* Counts the tracks of motion `motion` whose channel id `paramD` is `'H'` and whose `ref` equals
   `target` (n of them), then adds `vec` scaled by a per-track weight to every key of those tracks:
   the k-th matching track (k = 0, 1, …) gets weight (n − k)/n, so the first gets `vec` in full and
   the last `vec`/n. Key layout as in `BtlMotionDataOffsetTrackKeys`: keys start at the track's
   `data` plus the byte offset `table[i]`, `paramA` is the key count; bit 0x80 of `param8` selects
   8-byte half keys (`GmoMotionKeyH`, unpacked as with VFPU `vh2f`, re-packed with
   `BtlFloatToHalf`), otherwise 16-byte float keys (`GmoMotionKeyF`). Does nothing when no
   track matches. Used by `BtlMotionOffsetTrackKeysByName`. */
void BtlMotionDataScaleTrackKeysGraded(GmoMotionInfo *motion, u32 target, float *vec)
{
    const GmoMotionTrack *track;
    const u16 *keyOffset;
    float count;
    float remaining;
    int matches;
    int i;

    matches = 0;
    track = (const GmoMotionTrack *)PspPtr(motion->tracks);
    for (i = 0; i < motion->trackCount; i++, track++) {
        if (track->paramD == 'H' && track->ref == target) {
            matches++;
        }
    }
    if (matches == 0) {
        return;
    }

    count = (float)matches;
    remaining = count;
    track = (const GmoMotionTrack *)PspPtr(motion->tracks);
    keyOffset = (const u16 *)PspPtr(motion->table);
    for (i = 0; i < motion->trackCount; i++, track++, keyOffset++) {
        u8 *keys;
        float weight;
        float sx, sy, sz;
        int k;

        if (track->paramD != 'H' || track->ref != target) {
            continue;
        }
        weight = remaining / count;
        sx = vec[0] * weight;
        sy = vec[1] * weight;
        sz = vec[2] * weight;
        remaining = remaining - 1.0f;

        keys = (u8 *)PspPtr(track->data) + *keyOffset;
        if ((track->param8 & 0x80) != 0) {
            GmoMotionKeyH *key = (GmoMotionKeyH *)keys;

            for (k = 0; k < track->paramA; k++, key++) {
                float x = VfH2f(key->value[0]) + sx;
                float y = VfH2f(key->value[1]) + sy;
                float z = VfH2f(key->value[2]) + sz;

                key->value[0] = BtlFloatToHalf(x);
                key->value[1] = BtlFloatToHalf(y);
                key->value[2] = BtlFloatToHalf(z);
            }
        } else {
            GmoMotionKeyF *key = (GmoMotionKeyF *)keys;

            for (k = 0; k < track->paramA; k++, key++) {
                float x = key->value[0] + sx;
                float y = key->value[1] + sy;
                float z = key->value[2] + sz;

                key->value[0] = x;
                key->value[1] = y;
                key->value[2] = z;
            }
        }
    }
}
