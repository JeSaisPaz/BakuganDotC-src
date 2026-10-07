// bdc 0x089c542c SndDecOutThreadStep
#include "bdc.h"

/* One iteration of a decoder thread (`MyThread-Sound-Sub0..2`): produces and plays one 0x100-frame
   block, handles the suspend handshake, and reports whether the thread should keep running (1) or
   end (0, when the decoder is `finished`). */

s32 SndDecOutThreadStep(SndDecOut *dec)
{
    s32 keepRunning = 1;
    bool streamEnded = false;
    u8 *block;

    if (!dec->pauseRequested && SndBgmPlayerGet(dec->channel)->suspendRequested) {
        SndDecOutRequestPause(dec);
    }
    if (SndBgmPlayerIsDecoderStopRequested(SndBgmPlayerGet(dec->channel))) {
        dec->finished = 1;
    }

    if (dec->finished) {
        keepRunning = 0;
    } else if (dec->pauseRequested) {
        sceDisplayWaitVblankStartCB();
        if (!dec->paused) {
            /* Drain the hardware channel, then clear the buffers and acknowledge the pause. */
            while (SndWaveGetChannelRestLength(dec->channel) > 0) {
                sceDisplayWaitVblankStartCB();
            }
            CoreLockAcquire(dec->lock);
            block = (u8 *)dec->ring;
            memset(block, 0, SndGetManager()->blockBytes * 2);
            memset(dec->decodeBuf, 0, 0x2000);
            dec->blockCount = 0;
            dec->readBlock = 0;
            dec->paused = 1;
            SndBgmPlayerGet(dec->channel)->decoderQuiesced = 1;
            CoreLockRelease(dec->lock);
        } else if (dec->resumeRequested) {
            dec->pauseRequested = 0;
            dec->paused = 0;
            dec->resumeRequested = 0;
        }
    } else {
        CoreLockAcquire(dec->lock);
        dec->mode = dec->requestedMode;
        if (dec->mode == 2 || dec->mode == 6) {
            /* Movie modes: the block comes from the PSMF player's audio, or silence. */
            bool fromMovie = false;

            block = (u8 *)dec->ring;
            block += SndGetManager()->blockBytes * dec->readBlock;
            keepRunning = 1;
            if (GfxMovieHasPlayer() && GfxMoviePlayerIsActive((GfxMoviePlayer *)GfxMovieGetPlayer()) &&
                ((GfxMoviePlayer *)GfxMovieGetPlayer())->gotFrame) {
                fromMovie = true;
            }
            if (fromMovie) {
                GfxMoviePlayer *movie = (GfxMoviePlayer *)GfxMovieGetPlayer();

                GfxMoviePlayerReadAudio(movie, block, SndGetManager()->framesPerBlock);
            } else {
                memset(block, 0, SndGetManager()->blockBytes);
            }
            dec->blockCount = dec->blockCount + 1;
        } else if (dec->decodeEnabled) {
            if (SndDecOutDecodeBlock(dec) == 0) {
                if (dec->channel != 0) {
                    SndDecOutResetVolume(dec);
                    dec->decodeEnabled = 0;
                }
                streamEnded = true;
                dec->decodeEnabled = 0;
                SndDecOutSetVolume(dec, 0, 0);
            }
        } else {
            block = (u8 *)dec->ring;
            block += SndGetManager()->blockBytes * dec->readBlock;
            memset(block, 0, SndGetManager()->blockBytes);
            dec->blockCount = dec->blockCount + 1;
        }
        CoreLockRelease(dec->lock);
        SndDecOutOutputBlock(dec);
    }

    if (streamEnded) {
        SndBgmPlayerRequestStop(SndBgmPlayerGet(dec->channel));
    }
    if (SndGetManager()->stopFlag) {
        BootDeleteThread(dec->channel + 6);
    }
    return keepRunning;
}
