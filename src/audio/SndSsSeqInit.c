// bdc 0x08a218c8 SndSsSeqInit
#include "bdc.h"

/* Initialises the sequencer of the Sony sound layer: stores the output block size
   (`g_sndSsSeqBlockSize`, samples per block, used by `SndSsSeqTick`) and clears the 0x80
   sequence slots (`g_sndSsSeqTracks`). Returns 0, or -1 for a zero block size. */

s32 SndSsSeqInit(s32 blockSize)

{
  s32 i;

  if (blockSize == 0) {
    return -1;
  }
  g_sndSsSeqBlockSize = blockSize;
  for (i = 0; i < 0x80; i++) {
    g_sndSsSeqTracks[i] = NULL;
  }
  return 0;
}
