// bdc 0x089c39cc SndBgmPlayerStateStart
#include "bdc.h"

/* State 4 handler (entry 4 of `g_sndBgmPlayerStateTable`): attaches the prepared Atrac stream to
   the channel's decoder. First pass (`step == 0`): if no `SndDecOut` exists for the channel it
   clears `decoderStop` and creates one (`SndDecOutCreate`, mode 0 for channel 0, mode 5
   otherwise), leaving `step` at 0 so the next pass retries; if one exists it sets its base volume
   (the voice volume × 0.8 when `channel != 0` and the track id is ≥ 10000, else the BGM volume),
   resets the fade to 100 % (`SndDecOutSetVolume`), selects mode 0 on channel 0, hands over
   `atracId`, `buffer` and `bufferSize` with `SndDecOutSetStream`, then sets `step = 1` if the
   player is still in state 4 step 0. A pass with `step != 0` returns the player to idle
   (`state = 0`, `step = 0`). */

void SndBgmPlayerStateStart(SndBgmPlayer *player)
{
  s32 firstPass;
  s32 channel;
  s32 useVoice;
  float volume;

  firstPass = 0;
  CoreLockAcquire(player->lock);
  channel = player->channel;
  if (player->step != 0) {
    player->state = 0;
    player->step = 0;
  }
  else {
    firstPass = 1;
  }
  CoreLockRelease(player->lock);
  if (firstPass == 0) {
    return;
  }

  if (SndDecOutExists(channel) == 0) {
    player->decoderStop = 0;
    if (channel == 0) {
      SndDecOutCreate(channel, 0);
    }
    else {
      SndDecOutCreate(channel, 5);
    }
    return;
  }

  useVoice = 0;
  if (channel != 0 && player->trackId >= 10000) {
    useVoice = 1;
  }
  if (useVoice) {
    volume = SndManagerGetVoiceVolume(SndGetManager()) * 0.8f;
  }
  else {
    volume = SndManagerGetBgmVolume(SndGetManager());
  }
  SndDecOutSetBaseVolume(volume, SndDecOutGet(channel));
  SndDecOutSetVolume(SndDecOutGet(channel), 100, 0);
  if (channel == 0) {
    SndDecOutSetMode(SndDecOutGet(channel), 0);
  }
  SndDecOutSetStream(SndDecOutGet(channel), player->atracId, player->buffer, player->bufferSize);
  CoreLockAcquire(player->lock);
  if (player->state == 4 && player->step == 0) {
    player->step = player->step + 1;
  }
  CoreLockRelease(player->lock);
}
