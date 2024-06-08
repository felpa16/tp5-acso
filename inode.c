#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "inode.h"
#include "diskimg.h"


/**
 * TODO
 */
int inode_iget(struct unixfilesystem *fs, int inumber, struct inode *inp) {
    if (fs == NULL || inp == NULL) return -1;
    if (inumber < 1) return -1;
    int inode_size = sizeof(struct inode);
    int sector_num = INODE_START_SECTOR + (((inumber - 1) * inode_size) / DISKIMG_SECTOR_SIZE);
    struct inode* inodes = (struct inode*) malloc(DISKIMG_SECTOR_SIZE);
    if (inodes == NULL) return -1;
    if (diskimg_readsector(fs->dfd, sector_num, inodes) == -1) {
        free(inodes);
        return -1;
    }
    int index = (inumber - 1) % (DISKIMG_SECTOR_SIZE / inode_size);
    *inp = inodes[index];
    free(inodes);
    return 0;
}

/**
 * TODO
 */

int lookup_block_helper(struct unixfilesystem* fs, int blk_contain, int blk_index) {
    //unkown size of sector so heap.
    uint16_t* block_nums = (uint16_t*)malloc(DISKIMG_SECTOR_SIZE);
    int block_return = 0;
    if (diskimg_readsector(fs->dfd, blk_contain, block_nums) == -1)
            block_return =-1;
    if (block_return == 0) block_return = block_nums[blk_index];
    free(block_nums);
    return block_return;
}

int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum) {
    if (blockNum < 0) return -1;
    if (!(inp->i_mode & ILARG)) {
        if(blockNum > 7)  return -1;
        return inp->i_addr[blockNum];
    }
    //Block nums that fit in a sector
    int max_blocks = (DISKIMG_SECTOR_SIZE / sizeof(uint16_t));
    //Single indirection
    if (blockNum < 7 * max_blocks) {
        int index_inode = blockNum / max_blocks;
        int index_block = blockNum % max_blocks;
        return lookup_block_helper(fs, inp->i_addr[index_inode], index_block);
    }
    //Double indirection
    //Block index in double block system
    int remainingBlocs = blockNum - (7 * max_blocks);
    if (remainingBlocs >= max_blocks*max_blocks) return -1;
    int index_level_1 = remainingBlocs / max_blocks;
    int index_level_2 = remainingBlocs % max_blocks;
    int level_1 = lookup_block_helper(fs, inp->i_addr[7], index_level_1);
    if (level_1 == -1) return -1;
    return lookup_block_helper(fs, level_1, index_level_2);
}

// int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum) {

//     if (fs == NULL || inp == NULL) return -1;
//     int size = inode_getsize(inp);
//     int mod = size % DISKIMG_SECTOR_SIZE;
//     int block_count;
//     if (mod != 0) {
//         block_count = (size / DISKIMG_SECTOR_SIZE) + 1;
//     }
//     else {
//         block_count = size / DISKIMG_SECTOR_SIZE;
//     }
//     if (blockNum < 0 || blockNum > block_count) return -1;

//     int nums_per_block = DISKIMG_SECTOR_SIZE/sizeof(uint16_t);
//     int first_idx = blockNum/nums_per_block;
//     if (first_idx > 6) {
//         first_idx = 7;
//     }
//     int second_idx;
//     if (first_idx < 7) {
//         second_idx = blockNum - ((first_idx) * 256);
//         uint16_t sector_num = inp->i_addr[first_idx];
//         uint16_t* sector = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE);
//         if (diskimg_readsector(fs->dfd, sector_num, sector) == -1) {
//             free(sector);
//             return -1;
//         }
//         uint16_t result = sector[second_idx];
//         free(sector);
//         return result;
//     }
//     int third_idx;
//     second_idx = (blockNum - (7 * nums_per_block)) / nums_per_block;
//     third_idx = blockNum - (second_idx + 7) * nums_per_block;
//     uint16_t sector_num_1 = inp->i_addr[first_idx];
//     uint16_t* sector_1 = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE);
//     if (diskimg_readsector(fs->dfd, sector_num_1, sector_1) == -1) {
//         free(sector_1);
//         return -1;
//     }
//     uint16_t sector_num_2 = sector_1[second_idx];
//     free(sector_1);

//     uint16_t* sector_2 = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE);
//     if (diskimg_readsector(fs->dfd, sector_num_2, sector_2) == -1) {
//         free(sector_2);
//         return -1;
//     }
//     uint16_t result = sector_2[third_idx];
//     free(sector_2);
//     return result;
// }

// int inode_getsize(struct inode *inp) {
//   return ((inp->i_size0 << 16) | inp->i_size1); 
// }