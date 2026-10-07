// bdc 0x089d62f4 GfxMoviePlayerInit
#include "bdc.h"

/* Initialises the movie (PSMF/MPEG) player state: `sceMpegInit`, then allocates from the low end
   of the game heap `psmf` (4 bytes), `data` (12), `playInfo` (0x18, set to {codec 0xe, 0, 1, 0,
   0, 1}), `psmfInfo` (0x14) and `videoData` (12); builds the 300-byte GE vertex list `vertices`
   (30 through-mode {u, v, x, y, z} vertices = 15 sprites of 32x272 covering 480 pixels, drawn by
   `GfxMoviePlayerDrawFrame`), allocates the two 0x88000-byte aligned `frameBuffers`, the
   0x300000-byte PSMF buffer and the 0x2000-byte `audioBuffer`, and resets the state fields
   (`movieId` -1, `videoResult` 0x8061600C "no data"). No allocation result is checked. Called by
   `GfxMovieCreatePlayer`; undone by `GfxMoviePlayerRelease`. */

void GfxMoviePlayerInit(GfxMoviePlayer *player)

{
  bool fromLow;
  ScePsmfPlayer *psmf;
  ScePsmfPlayerData *data;
  ScePsmfPlayerPlayInfo *playInfo;
  ScePsmfPlayerPsmfInfo *psmfInfo;
  ScePsmfPlayerVideoData *videoData;
  u16 *verts;
  void *buf;
  s32 i;

  sceMpegInit();

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  psmf = MemAlloc(4, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->psmf = psmf;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0xc, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->data = data;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  playInfo = MemAlloc(0x18, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->playInfo = playInfo;
  playInfo->videoCodec = 0xe;
  player->playInfo->videoStreamNum = 0;
  player->playInfo->audioCodec = 1;
  player->playInfo->audioStreamNum = 0;
  player->playInfo->playMode = 0;
  player->playInfo->playSpeed = 1;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  psmfInfo = MemAlloc(0x14, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->psmfInfo = psmfInfo;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  videoData = MemAlloc(0xc, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->videoData = videoData;
  player->frameIndex = 0;
  player->active = 0;
  player->stopping = 0;
  player->frameReady = 0;
  player->stopRequest = 0;
  player->gotFrame = 0;
  player->firstFrame = 0;

  /* 30 vertices {u, v, x, y, z} (s16): sprite 0 is (0,0)-(0x20,0x110) at z 0xffff, every later
     vertex is the one two places back shifted right by 32 pixels (u and x). */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  verts = MemAlloc(300, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->vertices = verts;
  verts[0] = 0;
  player->vertices[1] = 0;
  player->vertices[2] = 0;
  player->vertices[3] = 0;
  player->vertices[4] = 0xffff;
  player->vertices[5] = 0x20;
  player->vertices[6] = 0x110;
  player->vertices[7] = 0x20;
  player->vertices[8] = 0x110;
  player->vertices[9] = 0xffff;
  for (i = 2; i < 30; i++) {
    verts = &player->vertices[i * 5];
    verts[0] = verts[-10];
    verts[1] = verts[-9];
    verts[2] = verts[-8];
    verts[3] = verts[-7];
    verts[4] = verts[-6];
    player->vertices[i * 5] += 0x20;
    player->vertices[i * 5 + 2] += 0x20;
  }

  player->frameBuffers[0] = MemAllocAligned(0x88000, true);
  player->frameBuffers[1] = MemAllocAligned(0x88000, true);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  buf = MemAlloc(0x300000, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->data->buffer = buf;
  player->data->bufferSize = 0x300000;
  player->initF14 = 0.0f;
  player->initF18 = 0.0f;
  player->initF1c = -1.0f;
  player->videoData->frameWidth = 0x200;
  player->videoData->displaybuf = NULL;
  player->videoData->displaypts = 0;
  player->videoResult = (s32)0x8061600C;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  buf = MemAlloc(0x2000, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  player->audioBuffer = buf;
  player->audioRemaining = 0;
  player->openRequest = 0;
  player->started = 0;
  player->movieId = -1;
  player->pts = 0;
}
