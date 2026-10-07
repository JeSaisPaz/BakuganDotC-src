// bdc 0x089c0208 SndEmitterGetProfile
#include "bdc.h"

/* Chooses the attenuation profile (`SndEmitterProfile`) of a sound id; both emitter constructors
   store the result in `SndEmitter.params`. It walks the 12-byte `{lo, hi, key}` range rows at
   `0x08aa12ac` (the rows are ids `0x02c00000..0x02c00070` -> key -2 and ids `0..3` -> key -1; the
   row count is the shared constant 2 at `0x08aa12c4`): for the first row with `lo <= soundId <= hi`
   whose key matches one of the first two profiles of `g_soundEmitterProfiles` it returns that
   profile; if no row matches it returns row 0 (key -1, "battle field"). */

SndEmitterProfile *SndEmitterGetProfile(SndListener *listener, s32 soundId) {
    SndEmitterProfile *result = NULL;
    s32 i;
    s32 j;

    for (i = 0; i < g_soundProfileCount; i++) {
        s32 *row = &g_soundProfileRanges[i * 3];

        if (soundId < row[0] || row[1] < soundId) {
            continue;
        }
        for (j = 0; j < g_soundProfileCount; j++) {
            if (row[2] == g_soundEmitterProfiles[j].key) {
                result = &g_soundEmitterProfiles[j];
                break;
            }
        }
        if (result != NULL) {
            break;
        }
    }
    if (result == NULL) {
        result = g_soundEmitterProfiles;
    }
    return result;
}
