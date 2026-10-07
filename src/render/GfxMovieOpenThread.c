// bdc 0x089d5ff0 GfxMovieOpenThread
#include "bdc.h"

/* Body of game thread 13 (started by `GfxMovieRequestPlay`): for the requested movie id `movieId`
   (0..0x77) and an idle player, creates the PSMF player (thread priority 0x2a), configures loop off
   and pixel type 3 (8888), resolves the file from `g_moviePathTable` with `IoMakePath`, sets
   it, reads the PSMF info and starts playback; marks the player active and started, clears the
   open request, registers `GfxMovieAudioTickCallback` and ends thread 13
   (`BootDeleteThread(0xd)`). Always clears `g_movieVideoErrorCount`. Returns 1 when a movie was
   started, 0 otherwise. */

s32 GfxMovieOpenThread(GfxMoviePlayer *player)
{
    char path[512];
    s32 id = player->movieId;
    s32 started = 0;

    if (id >= 0 && (u32)id < 0x78 && GfxMoviePlayerIsActive(player) == 0) {
        player->data->threadPriority = 0x2a;
        scePsmfPlayerCreate(player->psmf, player->data);
        scePsmfPlayerConfigPlayer(player->psmf, PSMF_PLAYER_CONFIG_MODE_LOOP, 1);
        scePsmfPlayerConfigPlayer(player->psmf, PSMF_PLAYER_CONFIG_MODE_PIXEL_TYPE, 3);
        IoMakePath(g_moviePathTable[id], path);
        scePsmfPlayerSetPsmf(player->psmf, path);
        scePsmfPlayerGetPsmfInfo(player->psmf, player->psmfInfo);
        scePsmfPlayerStart(player->psmf, player->playInfo, 0);
        player->active = 1;
        GfxMoviePlayerSetStopping(player, false);
        player->audioRemaining = 0;
        started = 1;
        player->frameReady = 0;
        GfxSetVblankHandler(0, GfxMovieAudioTickCallback, NULL);
        player->videoResult = (s32)0x8061600C;
        player->started = 1;
        player->openRequest = 0;
        BootDeleteThread(0xd);
    }
    g_movieVideoErrorCount = 0;
    return started;
}
