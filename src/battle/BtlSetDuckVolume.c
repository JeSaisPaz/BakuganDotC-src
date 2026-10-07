// bdc 0x0884dfc4 BtlSetDuckVolume
#include "bdc.h"

/* Sets BGM player 0's volume to `bgmVolume` with fade time `fadeTime` seconds
   (`SndBgmPlayerSetVolumeF`; skipped when that player does not exist) and, when the sound manager
   exists, sound category 2's volume to `sfxVolume` (`SndManagerSetCategoryVolume`; a
   one-iteration loop over categories 2..2). Used by `UiTalkShowMessage` to duck audio while a
   message is shown and by `UiTalkWindowStep` to restore it. */

void BtlSetDuckVolume(float bgmVolume, float sfxVolume, float fadeTime)
{
    int category;

    if (SndBgmPlayerExists(0)) {
        SndBgmPlayerSetVolumeF(bgmVolume, fadeTime, SndBgmPlayerGet(0));
    }
    for (category = 2; category < 3; category++) {
        if (SndHasManager()) {
            SndManagerSetCategoryVolume(sfxVolume, SndGetManager(), category);
        }
    }
}
