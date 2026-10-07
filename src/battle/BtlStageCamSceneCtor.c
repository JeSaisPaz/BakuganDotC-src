// bdc 0x089046f8 BtlStageCamSceneCtor
#include "bdc.h"

/* Constructor of the 0x1a4-byte stage camera scene: clears the file buffer pointer, then the
   0x60-byte first key block and the five 0x40-byte key blocks. Returns `scene`. */
BtlStageCamScene *BtlStageCamSceneCtor(BtlStageCamScene *scene)
{
    int i;

    scene->fileBuf = NULL;
    memset(scene->head, 0, sizeof(scene->head));
    for (i = 0; i < 5; i++) {
        memset(scene->blocks[i], 0, sizeof(scene->blocks[i]));
    }
    return scene;
}
