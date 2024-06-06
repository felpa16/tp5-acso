#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "inode.h"
#include "diskimg.h"


/**
 * TODO
 */
int inode_iget(struct unixfilesystem *fs, int inumber, struct inode *inp) {
    int inode_size = sizeof(struct inode);
    int sector_num = INODE_START_SECTOR + ((inumber * inode_size) / 512)
    struct inode* inodes = (struct inode*) malloc(DISKIMG_SECTOR_SIZE);
    if (diskimg_readsector(1, sector_num, inodes) == -1) {

    };

    return 0; 
}

/**
 * TODO
 */
int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp,
    int blockNum) {  
        //Implement code here
    return 0;
}

int inode_getsize(struct inode *inp) {
  return ((inp->i_size0 << 16) | inp->i_size1); 
}

int main(void) {
    int size = sizeof(struct inode);
    printf("%i", size);
}