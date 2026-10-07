// bdc 0x089c9764 ScriptSetup
#include "bdc.h"

/* Initialises a script object from a decoded package entry: stores the entry in `entry`; allocates
   (from the top of the heap) the track state array `tracks` (ScriptPackageEntry.trackCount, 8
   bytes each), a zeroed 32-bit table `vars` (varCount words, only if non-zero), a zeroed 0x500-byte
   `trackLocals` block and a zeroed bit table `flagBits` (flagBitCount bits rounded up to 32, only
   if non-zero). Each track state is set to wait 0, flags 1 (running) and the start offset taken
   from ScriptPackageEntry.trackStart. */

/* Allocate `size` bytes from the top of the game heap (temporarily clears the low-alloc policy). */
static inline void *ScriptAllocHigh(u32 size)
{
  bool fromLow;
  void *p;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(false);
  p = MemAlloc(size, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  return p;
}

void ScriptSetup(Script *script, void *entry)

{
  ScriptPackageEntry *hdr;
  int trackCount;
  u32 varCount;
  u32 flagBitCount;
  int i;

  script->entry = entry;
  trackCount = ((ScriptPackageEntry *)entry)->trackCount;
  script->tracks = ScriptAllocHigh(trackCount * sizeof(ScriptTrack));

  hdr = script->entry;
  varCount = hdr->varCount;
  if (varCount != 0) {
    u32 *vars = ScriptAllocHigh(varCount * 4);
    script->vars = vars;
    hdr = script->entry;
    memset(vars, 0, hdr->varCount * 4);
  }

  {
    ScriptTrackLocal *locals = ScriptAllocHigh(0x500);
    script->trackLocals = locals;
    memset(locals, 0, 0x500);
  }

  hdr = script->entry;
  flagBitCount = hdr->flagBitCount;
  if (flagBitCount != 0) {
    u32 *bits = ScriptAllocHigh(((int)(flagBitCount + 31) / 32) * 4);
    script->flagBits = bits;
    hdr = script->entry;
    memset(bits, 0, ((hdr->flagBitCount + 31) / 32) * 4);
  }

  for (i = 0; i < trackCount; i++) {
    script->tracks[i].flags = 1;
    script->tracks[i].pc = ((ScriptPackageEntry *)script->entry)->trackStart[i];
    script->tracks[i].wait = 0;
  }
}
