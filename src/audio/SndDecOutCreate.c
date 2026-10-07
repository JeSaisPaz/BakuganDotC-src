// bdc 0x089c4578 SndDecOutCreate
#include "bdc.h"

/* Creates the `SndDecOut` decoder/output channel `channel` (0..2) with initial output `mode`,
   unless the slot is already taken: lazily allocates the 0x10-byte `SndDecOutTable`
   (`g_soundDecOutTable`) from the heap bottom, allocates and constructs a 0xec-byte `SndDecOut`
   (`SndDecOutInit`), stores it in the slot, bumps the table count and sets its `channel`. If the
   matching `SndBgmPlayer` exists its `decoderQuiesced` flag is cleared, and the decoder thread
   `MyThread-Sound-Sub<channel>` (slot channel + 6 of `g_threadTable`) is started with
   `BootStartThread`, passing the channel number as its 4-byte argument. */

void SndDecOutCreate(s32 channel, s32 mode)
{
  bool fromLow;
  SndDecOutTable *table;
  SndDecOut *mem;
  SndDecOut *dec;
  s32 threadArg;

  if (g_soundDecOutTable == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    table = MemAlloc(sizeof(SndDecOutTable), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_soundDecOutTable = table;
    memset(table, 0, sizeof(SndDecOutTable));
  }
  if (g_soundDecOutTable->slots[channel] == NULL) {
    dec = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(SndDecOut), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      SndDecOutInit(mem, mode);
      dec = mem;
    }
    table = g_soundDecOutTable;
    table->slots[channel] = dec;
    table->count = table->count + 1;
    dec->channel = channel;
    if (SndBgmPlayerExists(channel)) {
      SndBgmPlayerGet(channel)->decoderQuiesced = 0;
    }
    memset(&threadArg, 0, sizeof(threadArg));
    threadArg = channel;
    BootStartThread(channel + 6, &threadArg, sizeof(threadArg));
  }
}
