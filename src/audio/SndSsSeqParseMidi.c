// bdc 0x08a21d20 SndSsSeqParseMidi
#include "bdc.h"

/* Validates a Standard MIDI File for the sequencer and binds it to `track`: requires the `MThd`
   chunk, format 0 and a non-zero division, then an `MTrk` chunk at `+0xe`; stores the division
   (ticks per quarter), the file, the track length and the event data pointer (`smf+0x16`) in
   `track`. Returns 0, or `0x80450005` for an unsupported file. */

s32 SndSsSeqParseMidi(const u8 *smf, SndSsSeqTrack *track)
{
    u16 division;

    if ((((u32)smf[0] << 24) | ((u32)smf[1] << 16) | ((u32)smf[2] << 8) | smf[3]) != 0x4d546864) {
        return 0x80450005;
    }
    if (smf[9] + smf[8] * 0x100 != 0) {
        return 0x80450005;
    }
    division = (u16)(smf[0xd] + smf[0xc] * 0x100);
    if (division == 0) {
        return 0x80450005;
    }
    if ((((u32)smf[0xe] << 24) | ((u32)smf[0xf] << 16) | ((u32)smf[0x10] << 8) | smf[0x11]) != 0x4d54726b) {
        return 0x80450005;
    }
    track->cursor = smf + 0x16;
    track->division = division;
    track->trackLength = ((u32)smf[0x12] << 24) + ((u32)smf[0x13] << 16) + ((u32)smf[0x14] << 8) + smf[0x15];
    track->smf = smf;
    return 0;
}
