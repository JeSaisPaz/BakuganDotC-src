// bdc 0x089ffcb4 CoreBufQueueCount
#include "bdc.h"

/* Returns the number of entries in the buffer queue (`CoreBufQueue`, vtable `0x08af59cc` at
   `+0x18`): `write - read`, or the capacity when the write index has wrapped below the read index.
    */
int CoreBufQueueCount(int *queue)
{
    /* queue[0] = capacity, queue[1] = read index, queue[2] = write index */
    int capacity = queue[0];
    int read = queue[1];
    int write = queue[2];

    if (write < read) {
        return capacity;
    }
    return write - read;
}
