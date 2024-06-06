#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "inode.h"
#include "diskimg.h"


int main(void) {
    printf("%i", DISKIMG_SECTOR_SIZE/sizeof(uint16_t));
    return 0;
}