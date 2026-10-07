// bdc 0x089c5218 SndDecOutOutputBlock
#include "bdc.h"

/* Plays the oldest filled ring block, blocking until the hardware accepts it. Under the lock it
   advances `readBlock`/`blockCount` and the fade counter, computes the volume
   (`SndDecOutGetVolume`) and resolves the output path: with `g_soundDecOutAltMode` set, or
   with two or more live decoders (`SndDecOutTable`.count), modes 0/1 become 3/4 and the
   per-channel path is used; otherwise a single decoder in a mode below 3 uses the stereo
   `sceAudioOutput2` path (`useOutput2`). Then the block is post-processed by mode — 1/4
   `SndSsRenderBlock(buf)`, 0/3 software scaling `SndSsRenderBlockWithMix(buf, v, v)` with
   `v = volume × 4096`, 2/5 and anything else (unsigned > 5) untouched — and sent either to
   `sceAudioOutput2OutputBlocking(vol, buf)` or, for the per-channel path, to
   `SndWaveOutputPannedBlocking(channel, vol, vol, buf)`. Hardware volume is `volume × 0x8000`
   unless the software scaling was applied with a non-negative `v` (then 0x8000).
   (The asm leaves 1 in v0; the only caller ignores it.) */

void SndDecOutOutputBlock(SndDecOut *dec)
{
  s32 mixVol;
  s32 hwVol;
  u8 useOutput2;
  s16 *buf;
  float volume;

  mixVol = -1;
  useOutput2 = 0;
  CoreLockAcquire(dec->lock);
  buf = dec->ring;
  buf = (s16 *)((u8 *)buf + SndGetManager()->blockBytes * dec->readBlock);
  if (dec->blockCount > 0) {
    dec->blockCount = dec->blockCount - 1;
    dec->readBlock = dec->readBlock + 1;
    if (dec->readBlock >= 2) {
      dec->readBlock = 0;
    }
  }
  if (dec->fadeLeft > 0) {
    dec->fadeLeft = dec->fadeLeft - 1;
  }
  volume = SndDecOutGetVolume(dec);
  if (g_soundDecOutAltMode != 0) {
    if (dec->mode < 2) {
      dec->mode = dec->mode + 3;
      dec->requestedMode = dec->mode;
    }
  } else {
    if (g_soundDecOutTable->count >= 2 && dec->mode < 2) {
      dec->mode = dec->mode + 3;
      dec->requestedMode = dec->mode;
    }
    if (g_soundDecOutTable->count < 2 && dec->mode < 3) {
      useOutput2 = 1;
    }
  }
  if (dec->useOutput2 != useOutput2) {
    dec->useOutput2 = useOutput2;
  }
  CoreLockRelease(dec->lock);

  switch ((u32)dec->mode) {
  case 0:
  case 3:
    mixVol = (s32)(volume * 4096.0f);
    SndSsRenderBlockWithMix(buf, mixVol, mixVol);
    break;
  case 1:
  case 4:
    SndSsRenderBlock(buf);
    break;
  default:
    break;
  }

  hwVol = 0x8000;
  if (mixVol < 0) {
    hwVol = (s32)(volume * 32768.0f);
  }
  if (useOutput2 != 0) {
    sceAudioOutput2OutputBlocking(hwVol, buf);
  } else {
    SndWaveOutputPannedBlocking(dec->channel, hwVol, hwVol, buf);
  }
}
