// bdc 0x089d5e4c GfxMoviePlayerReadAudio
#include "bdc.h"

/* Fills `count` stereo 16-bit samples (4 bytes each) of `out` with movie audio for the BGM decoder
   thread (`SndDecOutThreadStep` in movie modes): zero-fills `out` (when non-NULL), then, while
   playing with a frame ready, copies what is left in the 0x800-sample audio buffer (remaining count
   `audioRemaining`); when that does not cover `count`, refills it with `scePsmfPlayerGetAudioData`
   and copies the rest from the fresh block (nothing more if the refill fails). With `out` NULL it
   only advances `audioRemaining`. */

void GfxMoviePlayerReadAudio(GfxMoviePlayer *player, void *out, s32 count)
{
    u8 *dst = (u8 *)out;
    u8 *src;
    s32 remaining;
    s32 n;

    if (dst != NULL) {
        memset(dst, 0, count << 2);
    }
    if (GfxMoviePlayerIsActive(player) == 0 || player->frameReady == 0) {
        return;
    }
    remaining = player->audioRemaining;
    src = (u8 *)player->audioBuffer + (0x800 - remaining) * 4;
    n = count * 4;
    if (count >= remaining) {
        if (remaining > 0) {
            if (dst != NULL) {
                memcpy(dst, src, remaining * 4);
                dst += remaining * 4;
                n -= remaining * 4;
                remaining = player->audioRemaining;
            }
            count -= remaining;
            player->audioRemaining = 0;
        }
        if (scePsmfPlayerGetAudioData(player->psmf, player->audioBuffer) != 0) {
            n = 0;
        } else {
            src = (u8 *)player->audioBuffer;
            player->audioRemaining += 0x800;
        }
    }
    if (n > 0) {
        if (dst != NULL) {
            memcpy(dst, src, n);
        }
        player->audioRemaining -= count;
    }
}
