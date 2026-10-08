// bdc 0x08819a88 BtlScreenWaveDraw
#include "bdc.h"

/* Draws the full-screen wave distortion object (`BtlScreenWave`, `BtlMain` `screenWave`,
   drawn by `BtlMainDrawLoad` at layer 1.5) into render packet `packet`: opens a chunk
   (`GfxPacketBeginChunk`), calls the 2D state list, writes the screen camera
   `g_gfxScreenCamera` (`GfxCameraDlWrite`, all parts), uploads the 4x3 part of
   `g_gfxIdentityMatrix` as the world matrix (each float shifted to float24 and ORed with
   `g_btlScreenWaveWorldCmd`), binds the screen copy (`GfxGetFeedbackTexture`,
   `GfxTextureWriteCall`) and sets the material colour/alpha from `color` (VFPU: saturated to
   0..1, scaled by 255 and truncated to bytes). While `gridBuilt < 2`
   (once per frame buffer) it builds the `cols × rows` vertex grid covering 480×272 with UVs over
   256×256. Every frame it displaces the UVs: each vertex of column c >= 1 in row r gets
   `u = grid[c].u + table[i] * 0.05` (the row-0 vertex of that column) and
   `v = grid[r * cols].v + table[phase - i - 1] * 0.1`, where `i = r * cols + c` and `table =
   waveTable + phase`; column 0 is left as built. `grid` is
   `vertices + phase * g_gfxFrameIndex` for both the build and the displacement. It then draws `indexCount - 2` primitives of
   type 4 (index buffer `indices` when set) and closes the chunk (`GfxPacketEndChunk`). */

static inline u32 BtlScreenWaveFloat24(float value)
{
    u32 bits;

    memcpy(&bits, &value, sizeof(bits));
    return bits >> 8;
}

void BtlScreenWaveDraw(BtlScreenWave *wave, void *packet)
{
    const float *table = wave->waveTable + wave->phase;
    BtlScreenWaveVertex *grid = wave->vertices + wave->phase * g_gfxFrameIndex;
    const ScePspFVector4 *rows[4];
    u32 *list;
    u32 cmd;
    u32 rgba;
    s32 r;
    s32 c;
    s32 m;

    list = GfxPacketBeginChunk(packet);
    list = GfxDlCall2DState(list);
    list = GfxCameraDlWrite(g_gfxScreenCamera, list, 0xffffffff);
    list[0] = 0xdf000032;
    list[1] = 0xe0000000;
    list[2] = 0xe1000000;
    list[3] = 0x3a000000;
    cmd = g_btlScreenWaveWorldCmd & 0xff000000;
    rows[0] = &g_gfxIdentityMatrix.x;
    rows[1] = &g_gfxIdentityMatrix.y;
    rows[2] = &g_gfxIdentityMatrix.z;
    rows[3] = &g_gfxIdentityMatrix.w;
    for (m = 0; m < 4; m++) {
        list[4 + m * 3] = cmd | BtlScreenWaveFloat24(rows[m]->x);
        list[5 + m * 3] = cmd | BtlScreenWaveFloat24(rows[m]->y);
        list[6 + m * 3] = cmd | BtlScreenWaveFloat24(rows[m]->z);
    }
    list[16] = 0x23000000;
    list = GfxTextureWriteCall(GfxGetFeedbackTexture(), list + 17, 0);

    /* colour -> packed RGBA bytes: saturate to 0..1, scale by 255, truncate to bytes */
    rgba = (u32)VfI2uc(VfF2iz(VfSat0(wave->color[0]) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(wave->color[1]) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(wave->color[2]) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(wave->color[3]) * 255.0f, 23)) << 24;
    list[0] = (rgba & 0xffffff) | 0x55000000;
    list[1] = (rgba >> 24) | 0x58000000;
    list += 2;

    if (wave->gridBuilt < 2) {
        float colSpan;
        float rowSpan;
        float stepX;
        float stepY;
        float stepU;
        float stepV;
        float x;
        float y;

        wave->gridBuilt = wave->gridBuilt + 1;
        colSpan = (float)(wave->cols - 1);
        rowSpan = (float)(wave->rows - 1);
        stepX = 480.0f / colSpan;
        stepY = 272.0f / rowSpan;
        stepU = 256.0f / colSpan;
        stepV = 256.0f / rowSpan;
        y = 0.0f;
        for (r = 0; r < wave->rows; r++) {
            x = 0.0f;
            for (c = 0; c < wave->cols; c++) {
                BtlScreenWaveVertex *vtx = &grid[wave->cols * r + c];

                vtx->x = x;
                vtx->y = y;
                vtx->z = 0.0f;
                vtx->v = (float)r * stepV;
                vtx->u = (float)c * stepU;
                x = stepX + x;
            }
            y = stepY + y;
        }
    }

    for (r = 0; r < wave->rows; r++) {
        for (c = 1; c < wave->cols; c++) {
            s32 i = wave->cols * r + c;

            grid[i].u = grid[c].u + table[i] * 0.0500000007f;
            grid[i].v = grid[wave->cols * r].v + table[wave->phase - i - 1] * 0.100000001f;
        }
    }

    *list++ = 0x12801183;
    if (wave->indices != NULL) {
        u32 addr = PspAddr(wave->indices);

        list[0] = (u32)(((addr >> 24) & 0xf) << 16) | 0x10000000;
        list[1] = (u32)(addr & 0xffffff) | 0x02000000;
        list += 2;
    }
    if (grid != NULL) {
        u32 addr = PspAddr(grid);

        list[0] = (u32)(((addr >> 24) & 0xf) << 16) | 0x10000000;
        list[1] = (u32)(addr & 0xffffff) | 0x01000000;
        list += 2;
    }
    list[0] = (u32)(wave->indexCount - 2) | 0x04040000;
    list[1] = 0xe7000000;
    GfxPacketEndChunk(packet, list + 2);
}
