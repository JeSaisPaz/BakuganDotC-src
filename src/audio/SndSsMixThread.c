// bdc 0x08a23e30 SndSsMixThread
#include "bdc.h"

/* Output thread of the software mixer (libwave layer of `SndWaveInit`). Until `g_sndWaveQuit`
   is set it fills one half of `g_sndWaveMixBuf` per pass, in 16 slices of 28 stereo frames:
   for each playing voice of `g_sndWaveVoices` it refills `g_sndWaveDecodePtr` (one 16-byte
   VAG block decoded by `SndSsVagDecodeBlock`, or 0x38/0x70 bytes of mono/stereo 16-bit PCM),
   steps the voice's fade-out (applying the queued settings of `g_sndWaveVoiceShadow` when it
   ends) and adds the samples, scaled by volume and fade level, with saturation. When no voice
   played during the whole pass it first waits on `g_sndWaveEventFlag`; then it sends the block
   with `sceAudioOutputBlocking` on `argp->channel` and switches halves. On quit it outputs one
   silent (NULL) block and calls `sceKernelExitThread(0)`; the `return 0` is not reached. */

s32 SndSsMixThread(SceSize args, void *argp)
{
  SndWaveMixArgs *mixArgs = (SndWaveMixArgs *)argp;
  SndWaveVoice *v;
  SndWaveVoice *sh;
  const u8 *data;
  u8 *dec;
  s16 *pcm;
  s16 *out;
  u32 flags;
  u32 mode;
  u8 blockFlag;
  s32 buf;
  s32 slice;
  s32 i;
  s32 j;
  s32 played;
  s32 active;
  s32 chunk;
  s32 n;
  s32 rem;
  s32 level;
  s32 left;
  s32 right;

  buf = 0;
  while (g_sndWaveQuit == 0) {
    played = 0;
    for (slice = 0; slice < 16; slice++) {
      out = &g_sndWaveMixBuf[buf][slice * 56];
      sceKernelMemset(out, 0, 0x70);
      active = 0;
      for (i = 0; i < 8; i++) {
        v = &g_sndWaveVoices[i];
        if (v->playing == 0) {
          continue;
        }
        active++;
        flags = v->flags;
        if ((flags & 0x400) != 0) {
          /* no refill: mix whatever the decode buffer holds */
        } else if ((flags & 0x100) == 0) {
          /* VAG ADPCM: one 16-byte block -> 28 samples */
          data = v->data;
          if (data != NULL) {
            dec = (u8 *)g_sndWaveDecodePtr;
            sceKernelMemset(dec, 0, 0x70);
            mode = v->flags & 0xff;
            if (mode == 1 || mode == 2) {
              if (v->size >= 16) {
                if (v->size < v->pos + 16) {
                  if (mode == 2) {
                    v->pos = 0;
                    v->playing = 0;
                    goto mix;
                  }
                  v->pos = v->loopStart;
                }
                sceKernelMemcpy(g_sndWaveVagPtr, data + v->pos, 16);
                blockFlag = SndSsVagDecodeBlock((s16 *)dec, g_sndWaveVagPtr, v->history);
                if (blockFlag == 3) {
                  /* loop end: remember where, jump back to the loop start */
                  v->loopEnd = v->pos;
                  v->pos = v->loopStart;
                } else if (blockFlag == 6) {
                  /* loop start */
                  n = v->pos;
                  v->pos = n + 16;
                  v->loopStart = n;
                } else {
                  v->pos = v->pos + 16;
                }
              }
            } else if (mode == 0) {
              if (v->size < v->pos + 16) {
                v->pos = 0;
                v->playing = 0;
                goto mix;
              }
              sceKernelMemcpy(g_sndWaveVagPtr, data + v->pos, 16);
              v->pos = v->pos + 16;
              SndSsVagDecodeBlock((s16 *)dec, g_sndWaveVagPtr, v->history);
            }
          }
        } else {
          /* 16-bit PCM: 28 mono (0x38 bytes) or stereo (0x70 bytes) frames */
          data = v->data;
          if (data != NULL) {
            dec = (u8 *)g_sndWaveDecodePtr;
            chunk = ((flags & 0x200) != 0) ? 0x70 : 0x38;
            if ((flags & 1) == 0) {
              if (v->size < v->pos + chunk) {
                /* end of one-shot data: copy the tail and stop */
                sceKernelMemset(dec, 0, 0x70);
                sceKernelMemcpy(dec, data + v->pos, v->size - v->pos);
                v->pos = 0;
                v->playing = 0;
              } else {
                sceKernelMemcpy(dec, data + v->pos, chunk);
                v->pos = v->pos + chunk;
              }
            } else if (v->size < chunk) {
              /* looping sample shorter than one slice: repeat it until the slice is full */
              n = v->size - v->pos;
              sceKernelMemcpy(dec, data + v->pos, n);
              v->pos = 0;
              rem = chunk - n;
              while (v->size < rem) {
                sceKernelMemcpy(dec + n, data, v->size);
                n += v->size;
                rem -= v->size;
              }
              sceKernelMemcpy(dec + n, data, rem);
              v->pos = v->pos + rem;
            } else if (v->pos + chunk < v->size) {
              sceKernelMemcpy(dec, data + v->pos, chunk);
              v->pos = v->pos + chunk;
            } else {
              /* wrap: tail of the data, then its start */
              n = v->size - v->pos;
              sceKernelMemcpy(dec, data + v->pos, n);
              v->pos = 0;
              sceKernelMemcpy(dec + n, data, chunk - n);
              v->pos = chunk - n;
            }
          }
        }

      mix:
        pcm = g_sndWaveDecodePtr;
        sh = &g_sndWaveVoiceShadow[i];
        for (j = 0; j < 28; j++) {
          flags = v->flags;
          if ((flags & 0x100000) == 0) {
            v->level = 0x7f;
          } else {
            v->level = ((v->fadeLen - v->fadeCount) * 0x7f) / v->fadeLen;
            if (v->level < 0) {
              v->level = 0;
            }
            v->fadeCount = v->fadeCount + 1;
            if (v->fadeCount >= v->fadeLen) {
              /* fade-out done: apply the queued settings */
              flags &= ~0x100000;
              v->flags = flags;
              v->fadeCount = 0;
              if ((flags & 0x10000) != 0) {
                v->flags = flags & ~0x10000;
                v->playing = sh->playing;
                v->pos = sh->pos;
                v->volL = sh->volL;
                v->volR = sh->volR;
                v->history[0] = sh->history[0];
                v->history[1] = sh->history[1];
                v->field28 = sh->field28;
                v->field2c = sh->field2c;
              } else if ((flags & 0x20000) != 0) {
                v->flags = flags & ~0x20000;
                v->playing = sh->playing;
                v->pos = sh->pos;
                v->history[0] = sh->history[0];
                v->history[1] = sh->history[1];
                v->field28 = sh->field28;
                v->field2c = sh->field2c;
              } else if ((flags & 0x40000) != 0) {
                v->flags = flags & ~0x40000;
                v->playing = sh->playing;
              }
            }
          }
          level = v->level;
          if ((v->flags & 0x100) != 0 && (v->flags & 0x200) != 0) {
            left = ((v->volL * level) / 0x7f * pcm[j * 2]) / 4096;
            right = ((v->volR * level) / 0x7f * pcm[j * 2 + 1]) / 4096;
          } else {
            left = ((v->volL * level) / 0x7f * pcm[j]) / 4096;
            right = ((v->volR * level) / 0x7f * pcm[j]) / 4096;
          }
          left += out[j * 2];
          right += out[j * 2 + 1];
          if (left > 0x7fff) {
            left = 0x7fff;
          }
          if (right > 0x7fff) {
            right = 0x7fff;
          }
          if (left < -0x8000) {
            left = -0x8000;
          }
          if (right < -0x8000) {
            right = -0x8000;
          }
          out[j * 2] = (s16)left;
          out[j * 2 + 1] = (s16)right;
        }
      }
      played |= active;
    }
    if (played == 0) {
      sceKernelWaitEventFlag(g_sndWaveEventFlag, 1, 0x20, NULL, NULL);
    }
    sceAudioOutputBlocking(mixArgs->channel, 0x8000, g_sndWaveMixBuf[buf]);
    buf ^= 1;
  }
  sceAudioOutputBlocking(mixArgs->channel, 0, NULL);
  sceKernelExitThread(0);
  return 0;
}
