// bdc 0x089c4f6c SndDecOutDecodeBlock
#include "bdc.h"

/* Fills the next block of the output ring (`ring[(readBlock + blockCount) mod 2]`, 0x100 stereo
   frames = 0x400 bytes) from the decoded PCM. PCM comes from `decodeBuf`; when it runs dry the next
   ATRAC frame is decoded into it with `sceAtracDecodeData(atracId, decodeBuf, &samples,
   &finishFlag, &remain)`. Increments `blockCount`. Returns 1 normally and 0 when the decoder
   reports end of stream with no samples left; a decode error (or `atracId < 0`) zero-fills the
   remainder of the block and still returns 1. Does nothing when the ring already holds two blocks.
   Quirk kept from the binary: after a partial copy that follows a decode, the destination is not
   advanced, so the next copy overwrites the same samples. */

s32 SndDecOutDecodeBlock(SndDecOut *dec)
{
  s32 samples;
  s32 finished;
  s32 remainFrame;
  s32 result;
  s32 frames;
  s32 block;
  s32 err;
  s32 n;
  s16 *ring;
  s16 *buf;
  s16 *dst;
  s16 *src;

  samples = 0;
  finished = 0;
  remainFrame = 0;
  result = 1;
  if (dec->blockCount < 2) {
    frames = (SndGetManager()->framesPerBlock * 2) / 2;
    block = dec->readBlock + dec->blockCount;
    if (block >= 2) {
      block -= 2;
    }
    ring = dec->ring;
    dst = (s16 *)((u8 *)ring + SndGetManager()->blockBytes * block);
    buf = dec->decodeBuf;
    SndGetManager();
    src = buf + dec->decodePos * 2;
    while (frames > 0) {
      if (frames < dec->decodeRemain) {
        SndGetManager();
        memcpy(dst, src, frames * 4);
        dec->decodePos = dec->decodePos + frames;
        dec->decodeRemain = dec->decodeRemain - frames;
        frames = 0;
        continue;
      }
      if (dec->decodeRemain > 0) {
        SndGetManager();
        memcpy(dst, src, dec->decodeRemain * 4);
        SndGetManager();
        n = dec->decodeRemain;
        dec->decodePos = 0;
        dst += n * 2;
        frames -= n;
        dec->decodeRemain = 0;
      }
      if (dec->atracId < 0) {
        finished = 1;
        samples = 0;
        err = (s32)0x80630024;
      } else {
        err = sceAtracDecodeData(dec->atracId, (u16 *)dec->decodeBuf, &samples, &finished,
                                 &remainFrame);
        if (samples != 0) {
          dec->decodeRemain = samples;
        } else if (finished != 0) {
          result = 0;
        }
      }
      if (err != 0) {
        finished = 1;
        if (frames > 0) {
          dec->decodePos = 0;
          dec->decodeRemain = 0;
          SndGetManager();
          memset(dst, 0, frames * 4);
        }
        break;
      }
      if (frames <= 0) {
        break;
      }
      if (dec->decodeRemain >= frames) {
        buf = dec->decodeBuf;
        SndGetManager();
        memcpy(dst, buf, frames * 4);
        dec->decodePos = dec->decodePos + frames;
        dec->decodeRemain = dec->decodeRemain - frames;
        frames = 0;
      } else if (dec->decodeRemain > 0) {
        src = dec->decodeBuf;
        SndGetManager();
        memcpy(dst, src, dec->decodeRemain * 4);
        n = dec->decodeRemain;
        src = dec->decodeBuf;
        dec->decodePos = 0;
        frames -= n;
        dec->decodeRemain = 0;
      }
    }
    dec->blockCount = dec->blockCount + 1;
  }
  return result;
}
