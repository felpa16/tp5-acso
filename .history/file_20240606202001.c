#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "file.h"
#include "inode.h"
#include "diskimg.h"

/**
 * TODO
 */
int file_getblock(struct unixfilesystem *fs, int inumber, int blockNum, void *buf) {
    if (fs == NULL || buf == NULL) return -1;
    if (inumber < 1 || inumber > fs->superblock.s_ninode) return -1;
    struct inode* inode = (struct inode*) malloc(sizeof(struct inode));
    if (inode_iget(fs, inumber, inode) == -1) {
        free(inode);
        return -1;
    }
    int size = inode_getsize(inode);
    int mod = size % DISKIMG_SECTOR_SIZE;
    int block_count;
    if (mod != 0) {
        block_count = (size / DISKIMG_SECTOR_SIZE) + 1;
    }
    else {
        block_count = size / DISKIMG_SECTOR_SIZE;
    }
    if (blockNum < 0 || blockNum > block_count) return -1;

    int sector_num = inode_indexlookup(fs, inode, blockNum);
    if (diskimg_readsector(fs->dfd, sector_num, buf) == -1) return -1;

    if (blockNum < block_count) return DISKIMG_SECTOR_SIZE

    return 0;
}

