// bdc 0x0880ce78 SaveProfileReset
#include "bdc.h"

/* Resets a player profile to factory defaults: zero-fills the profile's 0xed0-byte save block
   (`self->data`), stamps `magic = 1` and `sizeStamp = SaveGetDataBlockSize() + 2`, stores the
   default name `"New Player"`, sets the BGM/SE/voice volumes to 10, zeroes the points, resets the
   loadouts (`SaveProfileResetLoadouts`), opens areas 0 and 9 (unlock mask and visited bits),
   clears the stage states, field counter, event/unlock flags and movie bytes, fills the stage actor
   tags and the `resetFFBlock` with 0xff, clears `eventFlagsStored`, sets script globals 4..7
   (`g_scriptGlobalVars`) and profile words 4..6 to -1, clears all records
   (`SaveProfileClearRecord` 0) and forgets the save slot (`SysUtilResetSaveSlotIndex`). Does
   nothing when `self->data` is NULL. */

void SaveProfileReset(SaveProfile *self)
{
    if (self->data != NULL) {
        memset(self->data, 0, 0xed0);
        self->data->magic = 1;
        self->data->sizeStamp = SaveGetDataBlockSize() + 2;
        UiTextEncodeSjis((u8 *)self->data->playerName, "New Player");
        SaveProfileSetBgmVolume(self, 10);
        SaveProfileSetSeVolume(self, 10);
        SaveProfileSetVoiceVolume(self, 10);
        self->data->points = 0;
        SaveProfileResetLoadouts(self);
        self->data->storyAreaMask |= 1;
        self->data->areaVisited[0] |= 1;
        self->data->storyAreaMaskHi |= 2;
        self->data->areaVisited[1] |= 2;
        memset(self->data->stageStates, 0, 0x80);
        memset(&self->data->fieldCounter, 0, 1);
        memset(self->data->eventFlags, 0, 0x108);
        memset(self->data->unlockFlags, 0, 0x10);
        memset(self->data->mapMovieWatched, 0, 0x50);
        memset(self->data->stageActorTags, -1, 0x140);
        self->data->eventFlagsStored = 0;
        memset(self->data->resetFFBlock, 0xff, 0x3e);
        g_scriptGlobalVars[4] = -1;
        g_scriptGlobalVars[5] = -1;
        g_scriptGlobalVars[6] = -1;
        g_scriptGlobalVars[7] = -1;
        SaveProfileSetWord(SaveGetProfile(), 4, 0xffffffff);
        SaveProfileSetWord(SaveGetProfile(), 5, 0xffffffff);
        SaveProfileSetWord(SaveGetProfile(), 6, 0xffffffff);
        SaveProfileClearRecord(self, 0);
        SysUtilResetSaveSlotIndex();
    }
}
