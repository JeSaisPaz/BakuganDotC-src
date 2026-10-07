// bdc 0x089f21b8 GfxRenderInit
#include "bdc.h"

/* Initialises the render layer for a new display: frees any earlier buffers
   (`GfxRenderShutdown`), runs the texture-system init (`GfxTextureSystemReset`, which creates
   `"FeedBackTex"`) and the screen camera (`GfxScreenCameraCreate`), allocates 0xc0040 bytes
   from the low heap for two 0x60000-byte frame display lists (aligned to 0x40; `g_gfxFrameIndex`
   = current half, `g_renderListCursor` = write pointer), `sceGuStart`s context 4 on the first
   half with PSM/FBW/FBP commands and `GfxSetupGeState`, then allocates the packet pool
   `g_renderPacketPool` (0xd04 bytes = 0x34-byte packets, see `GfxNewRenderPacket`) and the
   display-chunk node pool `g_renderChunkPool` (0x1b04 bytes = 0x24-byte nodes), resets the
   packet list `g_renderPacketList` and stores the default light/colour constants. */

void GfxRenderInit(void)
{
  bool wasLow;
  void *mem;

  GfxRenderShutdown();
  GfxTextureSystemReset();
  GfxScreenCameraCreate();

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0xc0040 /* PSP: two 0x60000-byte display lists + 0x40 alignment slack */, NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  g_renderListBuf = mem;
  g_gfxFrameIndex = 0;
  g_renderListBuffer = (u32 *)(((uintptr_t)mem + 0x3f) & ~(uintptr_t)0x3f);
  g_renderListCursor = g_renderListBuffer;
  g_renderListStart = g_renderListBuffer;
  sceGuStart(4, g_renderListBuffer, 0x60000);
  g_renderListCursor = GfxGuGetListPtr(NULL);
  g_renderListCursor[0] = 0xd2000003; /* PSM 8888 */
  g_renderListCursor[1] = 0x9d000200; /* FBW 512 */
  g_renderListCursor[2] = 0x9c000000; /* FBP 0 */
  g_renderListCursor = g_renderListCursor + 3;
  sceGuSetMemory(g_renderListCursor);
  GfxSetupGeState();
  g_renderListCursor = GfxGuGetListPtr(NULL);

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(64 * sizeof(RenderPacket) + 4 /* PSP: extra trailing word */, NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  g_renderPacketPool = mem;
  g_renderPacketCount = 0;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(192 * sizeof(CoreNode) + 4 /* PSP: extra trailing word */, NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  g_renderChunkPool = (CoreNode *)mem;
  g_renderChunkPoolCount = 0;

  GmoSystemInit();
  GfxDeferredDeleteInit();

  g_renderPacketList.tail = NULL;
  g_renderPacketList.head = NULL;
  g_renderPacketList.count = 0;

  g_gfxLightDir0[0] = 4.3f;
  g_gfxLightDir0[1] = 4.125f;
  g_gfxLightDir0[2] = 3.78f;
  g_gfxLightDir0[3] = 0.0f;
  g_gfxLightColor0[0] = 0.8f;
  g_gfxLightColor0[1] = 0.8f;
  g_gfxLightColor0[2] = 0.8f;
  g_gfxLightColor0[3] = 1.0f;
  g_gfxLightDir1[0] = -4.3f;
  g_gfxLightDir1[1] = 2.125f;
  g_gfxLightDir1[2] = 3.78f;
  g_gfxLightDir1[3] = 0.0f;
  g_gfxLightColor1[0] = 0.2f;
  g_gfxLightColor1[1] = 0.2f;
  g_gfxLightColor1[2] = 0.2f;
  g_gfxLightColor1[3] = 1.0f;
  g_gfxAmbientColor[0] = 0.4f;
  g_gfxAmbientColor[1] = 0.4f;
  g_gfxAmbientColor[2] = 0.4f;
  g_gfxAmbientColor[3] = 1.0f;
}
