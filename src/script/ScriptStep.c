// bdc 0x089c92f4 ScriptStep
#include "bdc.h"

/* Runs one frame of a script. The script's package entry (`+0x24`, `ScriptPackageEntry`) holds a
   track count and a table of per-track start offsets, followed by the bytecode; `+0x28` holds one
   `ScriptTrack` per track {s32 wait frames, u16 flags (bit 0 = running), u16 pc}. For each
   running track a non-zero wait is decremented; otherwise the bytecode at `pc` is fetched with
   `ScriptFetchWord` and its low byte is dispatched through vtable entry 2 of the script class
   (the opcode interpreter). The handler's return value controls the track: 1 advance `pc` by the
   opcode length (`Script.length`, `+0x41`) and end this track's turn, 2 end the turn without
   advancing, 3 `pc` was changed by the handler (re-fetch), 4 stop the track, 5 stop the track and
   finish the whole script; anything else advances `pc` and continues with the next opcode.
   Returns -1 when an opcode asked to end the script, otherwise 0; a paused script (`+0x54`) or one
   without an entry does nothing. */

int ScriptStep(Script *script)

{
  ScriptPackageEntry *entry;
  const VtblEntry *vtbl;
  ScriptTrack *track;
  u8 *code;
  u32 word;
  u32 pc;
  int count;
  int i;
  int result;

  entry = (ScriptPackageEntry *)script->entry;
  if (entry == NULL || script->paused != 0) {
    return 0;
  }
  count = entry->trackCount;
  code = (u8 *)&entry->trackStart[count];
  script->curTrack = script->tracks;
  for (i = 0; i < count; i++) {
    track = script->curTrack;
    script->track = (u8)i;
    if ((track->flags & 1) != 0) {
      if (track->wait != 0) {
        track->wait = track->wait - 1;
      } else {
        pc = track->pc;
        for (;;) {
          script->operand = code + pc;
          word = ScriptFetchWord(script);
          script->opcode = (u8)word;
          script->length = (u8)(word >> 8);
          script->refMask = (u16)(word >> 16);
          script->operandIdx = 0;
          vtbl = (const VtblEntry *)script->vtable;
          result = ((int (*)(void *, u8))vtbl[2].fn)((u8 *)script + vtbl[2].delta,
                                                      script->opcode);
          track = script->curTrack;
          if (result == 1) {
            track->pc = track->pc + script->length;
            break;
          }
          if (result == 2) {
            break;
          }
          if (result == 4) {
            track->flags = track->flags & ~1;
            break;
          }
          if (result == 5) {
            track->flags = track->flags & ~1;
            return -1;
          }
          if (result != 3) {
            track->pc = track->pc + script->length;
          }
          pc = script->curTrack->pc;
        }
      }
    }
    script->curTrack = script->curTrack + 1;
  }
  return 0;
}
