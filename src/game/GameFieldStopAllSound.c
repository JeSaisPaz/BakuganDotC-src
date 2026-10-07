// bdc 0x088be320 GameFieldStopAllSound
#include "bdc.h"

/* Stops all field sound: BGM queue 0 (0.4 s) and 1 (0.5 s) (`SndBgmQueueStop`), fades out all
   voices (`SndManagerFadeOutAllVoices`), cancels BGM channel 1 and releases every sound
   (`SndReleaseAll`). */
void GameFieldStopAllSound(void)
{
    SndBgmQueueStop(0.4f, 0);
    SndManagerFadeOutAllVoices(SndGetManager());
    SndBgmCancelChannel(1);
    SndBgmQueueStop(0.5f, 1);
    SndReleaseAll();
}
