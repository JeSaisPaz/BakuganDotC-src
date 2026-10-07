// bdc 0x089c94ec ScriptGetSourceLine
#include "bdc.h"

/* Maps the current position of a script to a source line number for diagnostics: it steps over the
   instructions of the first track from its start offset up to `tracks[0].pc`, using each
   instruction's length byte (the 4-byte instruction header is copied byte-wise, it may be unaligned),
   and returns in `*line` the matching entry of the package entry's line
   table (unaligned little-endian u16 table at `lineTableOff`, indexed from 1); `*line` is 0xffffffff
   when the entry has no line table. Returns 1 when at least one instruction was visited, otherwise
   0 (and 0 when the script has no entry). */

int ScriptGetSourceLine(Script *script, u32 *line)
{
    ScriptPackageEntry *entry = (ScriptPackageEntry *)script->entry;
    ScriptTrack *track;
    u8 *code;
    u8 *end;
    u8 *p;
    u8 *lines;
    int ret = 0;
    int idx = 1;
    int i;

    if (entry != NULL) {
        *line = 0;
        entry = (ScriptPackageEntry *)script->entry;
        track = script->tracks;
        code = (u8 *)&entry->trackStart[entry->trackCount];
        for (i = 0; i < 1; i++) {
            end = code + track->pc;
            p = code + ((ScriptPackageEntry *)script->entry)->trackStart[i];
            if (!(end < p)) {
                lines = (u8 *)entry + entry->lineTableOff + idx * 2;
                do {
                    union {
                        u32 word;
                        u8 b[4];
                    } hdr;
                    u32 v = 0xffffffff;
                    hdr.word = (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
                    if (((ScriptPackageEntry *)script->entry)->lineTableOff != 0) {
                        v = (u32)lines[0] | ((u32)lines[1] << 8);
                    }
                    *line = v;
                    idx++;
                    p += hdr.b[1];
                    lines += 2;
                    ret = 1;
                } while (!(end < p));
            }
            track++;
        }
    }
    return ret;
}
