// bdc 0x089d6c44 GfxMovieTaskDraw
#include "bdc.h"

/* Draw method of the movie task (id 10040): opens a depth-999 render packet and draws the current
   video frame (`GfxMoviePlayerDraw`); while skipping, overlays a full-screen (480×272) rectangle
   in the clear colour with alpha `1 - fade` (`GfxPacketDrawRect`) above the active fader. */

void GfxMovieTaskDraw(CoreTask *task)
{
  GfxMovieTask *self = (GfxMovieTask *)task;
  void *packet;
  float color[4];

  if (g_gfxMovieRectInit == 0) {
    g_gfxMovieRectInit = 1;
    g_gfxMovieRect[0] = 0.0f;
    g_gfxMovieRect[1] = 0.0f;
    g_gfxMovieRect[2] = 480.0f;
    g_gfxMovieRect[3] = 272.0f;
  }
  GfxNewRenderPacket(999.0f);
  if (GfxMovieHasPlayer()) {
    GfxMoviePlayerDraw(GfxMovieGetPlayer());
  }
  if (self->skipping != 0) {
    packet = GfxNewRenderPacket(GfxGetActiveFader()->sortKey + 10.0f);
    color[0] = g_gfxDisplay->clearColor[0];
    color[1] = g_gfxDisplay->clearColor[1];
    color[2] = g_gfxDisplay->clearColor[2];
    color[3] = 1.0f - self->alpha;
    GfxPacketDrawRect(packet, g_gfxMovieRect, color);
  }
}
