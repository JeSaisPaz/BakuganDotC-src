// bdc 0x089c6240 SndManagerResolveSoundWord
#include "bdc.h"

/* Resolves a queued sound word (see the layout in `SndManagerIsSoundPlaying`) to the
   `SndGroupSlot` that holds its sound bank and the sound index inside that bank. If the slot
   field (bits 27..31) is 0 it searches the 32 slots for the one whose `groupId` equals the group id
   in bits 20..26 (slot -1 if none); then it accepts the result only when the slot's bank is loaded
   (`bankId >= 0`) and the index (bits 0..19) lies inside the bank's sound range `[firstSound,
   lastSound]` (`s16`s at slot `+0x424` / `+0x426`, read from the PPTN block of the `.phd` header by
   `SndManagerUpdateGroupSlots`). On success it stores the slot and the index through `outSlot` /
   `outIdx` (either may be NULL); otherwise it leaves both untouched, so the caller presets `outSlot
   = -1`. */

void SndManagerResolveSoundWord(SndManager *mgr, u32 soundWord, s32 *outSlot, u32 *outIdx)

{
  u32 idx = soundWord & 0xfffff;
  s32 slot = soundWord >> 27;
  s32 groupId = (soundWord & 0x7f00000) >> 20;
  bool ok = false;
  s32 i;

  if (slot == 0) {
    slot = -1;
    for (i = 0; i < 32; i++) {
      if (mgr->groups[i].groupId == groupId) {
        slot = i;
        break;
      }
    }
  }
  if (slot >= 0 && slot < 32 && mgr->groups[slot].bankId >= 0 &&
      (s32)idx >= mgr->groups[slot].firstSound && (s32)idx <= mgr->groups[slot].lastSound) {
    ok = true;
  }
  if (ok) {
    if (outSlot != NULL) {
      *outSlot = slot;
    }
    if (outIdx != NULL) {
      *outIdx = idx;
    }
  }
}
