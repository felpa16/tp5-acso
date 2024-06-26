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
    if (inumber < 1 || inumber > fs->superblock.s_ninode) return -1;
    int inode_size = sizeof(struct inode);
    int sector_num = INODE_START_SECTOR + (((inumber - 1) * inode_size) / DISKIMG_SECTOR_SIZE);
    struct inode* inodes = (struct inode*) malloc(DISKIMG_SECTOR_SIZE);
    // int fd = diskimg_open("/usr", 1);
    if (diskimg_readsector(fs->dfd, sector_num, inodes) == -1) return -1;
    int index = (inumber - 1) % (DISKIMG_SECTOR_SIZE / inode_size);
    *inp = inodes[index];
    free(inodes); // no estoy seguro si puedo hacer este free. O sea, inp copia lo que habia en inodes[index] o apunta ahi nomas? Si solo apunta no puedo hacer el free. Si lo copia si puedo
    return 0;
}

/**
 * TODO
 */
int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum) {

    if (fs == NULL || inp == NULL) return -1;
    int size = inode_getsize(inp);
    int mod = size % DISKIMG_SECTOR_SIZE;
    int block_count;
    if (mod != 0) {
        block_count = (size / DISKIMG_SECTOR_SIZE) + 1;
    }
    else {
        block_count = size / DISKIMG_SECTOR_SIZE;
    }
    if (blockNum < 0 || blockNum > block_count) return -1;

    int is_small = inp->i_mode & ILARG;
    if (is_small) {
        return inp->i_addr[blockNum];
    }
    
    int nums_per_block = DISKIMG_SECTOR_SIZE/sizeof(uint16_t);
    int first_idx = blockNum/nums_per_block;
    if (first_idx > 6) {
        first_idx = 7;
    }
    

    return 0;
}

int inode_getsize(struct inode *inp) {
  return ((inp->i_size0 << 16) | inp->i_size1); 
}

int main(void) {
    int size = sizeof(struct inode);
    printf("%i", size);
}