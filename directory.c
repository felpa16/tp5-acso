#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * TODO
 */
int directory_findname(struct unixfilesystem *fs, const char *name, int dirinumber, struct direntv6 *dirEnt) {
    if (fs == NULL || name == NULL || dirEnt == NULL || dirinumber < 1) return -1;

    struct inode* inode = (struct inode*) malloc(sizeof(struct inode));
    if (inode == NULL) return -1;

    if (inode_iget(fs, dirinumber, inode) == -1) {
        free(inode);
        return -1;
    }

    int size = inode_getsize(inode);
    int block_count = (size + DISKIMG_SECTOR_SIZE - 1) / DISKIMG_SECTOR_SIZE;

    struct direntv6* dirents = (struct direntv6*) malloc(DISKIMG_SECTOR_SIZE);
    if (dirents == NULL) {
        free(inode);
        return -1;
    }

    if (block_count <= 7) {

        for (int i = 0; i < block_count; i++) {
            if (diskimg_readsector(fs->dfd, inode->i_addr[i], dirents) == -1) {
                free(dirents);
                free(inode);
                return -1;
            }
            for (unsigned long int j = 0; j < DISKIMG_SECTOR_SIZE / sizeof(struct direntv6); j++) {
                if (strcmp(dirents[j].d_name, name) == 0) {
                    *dirEnt = dirents[j];
                    free(dirents);
                    free(inode);
                    return 0;
                }
            }
        }
    } else {

        uint16_t* sector_nums = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE);
        if (sector_nums == NULL) {
            free(dirents);
            free(inode);
            return -1;
        }

        for (unsigned long int i = 0; i < 7; i++) {
            if (diskimg_readsector(fs->dfd, inode->i_addr[i], sector_nums) == -1) {
                free(sector_nums);
                free(dirents);
                free(inode);
                return -1;
            }
            for (unsigned long int j = 0; j < DISKIMG_SECTOR_SIZE / sizeof(uint16_t); j++) {
                if (diskimg_readsector(fs->dfd, sector_nums[j], dirents) == -1) {
                    free(sector_nums);
                    free(dirents);
                    free(inode);
                    return -1;
                }
                for (unsigned long int k = 0; k < DISKIMG_SECTOR_SIZE / sizeof(struct direntv6); k++) {
                    if (strcmp(dirents[k].d_name, name) == 0) {
                        *dirEnt = dirents[k];
                        free(sector_nums);
                        free(dirents);
                        free(inode);
                        return 0;
                    }
                }
            }
        }

        if (diskimg_readsector(fs->dfd, inode->i_addr[7], sector_nums) == -1) {
            free(sector_nums);
            free(dirents);
            free(inode);
            return -1;
        }

        uint16_t* sector_nums_2 = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE);
        if (sector_nums_2 == NULL) {
            free(sector_nums);
            free(dirents);
            free(inode);
            return -1;
        }

        for (unsigned long int i = 0; i < DISKIMG_SECTOR_SIZE / sizeof(uint16_t); i++) {
            if (diskimg_readsector(fs->dfd, sector_nums[i], sector_nums_2) == -1) {
                free(sector_nums_2);
                free(sector_nums);
                free(dirents);
                free(inode);
                return -1;
            }
            for (unsigned long int j = 0; j < DISKIMG_SECTOR_SIZE / sizeof(uint16_t); j++) {
                if (diskimg_readsector(fs->dfd, sector_nums_2[j], dirents) == -1) {
                    free(sector_nums_2);
                    free(sector_nums);
                    free(dirents);
                    free(inode);
                    return -1;
                }
                for (unsigned long int k = 0; k < DISKIMG_SECTOR_SIZE / sizeof(struct direntv6); k++) {
                    if (strcmp(dirents[k].d_name, name) == 0) {
                        *dirEnt = dirents[k];
                        free(sector_nums_2);
                        free(sector_nums);
                        free(dirents);
                        free(inode);
                        return 0;
                    }
                }
            }
        }
        free(sector_nums_2);
        free(sector_nums);
    }

    free(dirents);
    free(inode);
    return -2;
}