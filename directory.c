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
// int directory_findname(struct unixfilesystem *fs, const char *name, int dirinumber, struct direntv6 *dirEnt) {


//   // USAR FILE_GETBLOCK PARA VER LA CANTIDAD DE BYTES VALIDOS DE UN BLOQUE Y NO LEER DE MAS
//   printf("Se llama a directory_findname con name %s y dirinumber %i\n", name, dirinumber);

//   if (fs == NULL || name == NULL || dirEnt == NULL) return -1;
//   if (dirinumber < 1) return -1;

//   struct inode* inode = (struct inode*) malloc(DISKIMG_SECTOR_SIZE);
//   if (inode == NULL) {
//     free(inode);
//     return -1;
//   }
//   if (inode_iget(fs, dirinumber, inode) == -1) {
//     free(inode);
//     return -1;
//   }

//   int size = inode_getsize(inode);
//   printf("Tamaño del directorio: %i bytes\n", size);
//   int mod = size % DISKIMG_SECTOR_SIZE;
//   int block_count;
//   if (size <= 8 * DISKIMG_SECTOR_SIZE) { // small directory
//     printf("Es un small directory\n");
//     if (mod != 0) {
//         block_count = (size / DISKIMG_SECTOR_SIZE) + 1;
//     }
//     else {
//         block_count = size / DISKIMG_SECTOR_SIZE;
//     }

//     struct direntv6* dirents = (struct direntv6*) malloc(DISKIMG_SECTOR_SIZE);
//     if (dirents == NULL) {
//       free(dirents);
//       free(inode);
//       return -1;
//     }
//     for (int i = 0; i < block_count; i++) {
//       if (diskimg_readsector(fs->dfd, inode->i_addr[i], dirents) == -1) {
//         free(dirents);
//         free(inode);
//         return -1;
//       }
//       for (long unsigned int j = 0; j < DISKIMG_SECTOR_SIZE/sizeof(struct direntv6); j++) {
//         if (strcmp(dirents[j].d_name, name) == 0) {
//           printf("1\n");
//           *dirEnt = dirents[j];
//           free(dirents);
//           free(inode);
//           return 0;
//         }
//       }
//     free(dirents);
//     free(inode);
//     dirEnt = NULL; // ojo igual con esto porque podria llegar a dar seg faults? pero creo que queremos que de seg fault en este caso
//     return -2;
//     }
//   }

//   // caso en el cual es un large directory
//   bool double_indirec = false;
//   if (size < 7 * 256 * DISKIMG_SECTOR_SIZE) {
//     if (mod != 0) {
//       block_count = (size / (256 * DISKIMG_SECTOR_SIZE)) + 1;
//     }
//     else {
//       block_count = size / (256 * DISKIMG_SECTOR_SIZE);
//     }
//   }
//   else {
//     block_count = 7;
//     double_indirec = true;
//   }

//   uint16_t* sector_nums = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE); // chequear si falla el malloc
//   struct direntv6* dirents = (struct direntv6*) malloc(DISKIMG_SECTOR_SIZE);
//   if (sector_nums == NULL || dirents == NULL) {
//     free(sector_nums);
//     free(dirents);
//     free(inode);
//     return -1;
//   }
//   for (int i = 0; i < block_count; i++) {

//     if (diskimg_readsector(fs->dfd, inode->i_addr[i], sector_nums) == -1) {
//       free(sector_nums);
//       free(dirents);
//       free(inode);
//       return -1;
//     }
//     for (long unsigned int j = 0; j < DISKIMG_SECTOR_SIZE/sizeof(uint16_t); j++) { // no siempre va a medir 256, se puede optimizar para que solo itere hasta la cantidad de numeros que haya
//       if (diskimg_readsector(fs->dfd, sector_nums[j], dirents) == -1) {
//         free(sector_nums);
//         free(dirents);
//         free(inode);
//         return -1;
//       }
//       for (long unsigned int k = 0; k < DISKIMG_SECTOR_SIZE/sizeof(struct direntv6); k++) {
//         if (strcmp(dirents[k].d_name, name) == 0) {
//           printf("2\n");
//           *dirEnt = dirents[k];
//           free(sector_nums);
//           free(dirents);
//           free(inode);
//           return 0;
//         }
//       }
//     } 
//   }
//   if (double_indirec == false) {
//     free(sector_nums);
//     free(dirents);
//     free(inode);
//     dirEnt = NULL;
//     return -2;
//   }
//   if (diskimg_readsector(fs->dfd, inode->i_addr[7], sector_nums) == -1) {
//     free(sector_nums);
//     free(dirents);
//     free(inode);
//     return -1;
//   }
//   uint16_t* sector_nums_2 = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE); // chequear el malloc
//   if (sector_nums_2 == NULL) {
//     free(sector_nums_2);
//     free(sector_nums);
//     free(dirents);
//     free(inode);
//     return -1;
//   }
//   for (long unsigned int i = 0; i < DISKIMG_SECTOR_SIZE/sizeof(uint16_t); i++) {
//     if (diskimg_readsector(fs->dfd, sector_nums[i], sector_nums_2) == -1) {
//       free(sector_nums_2);
//       free(sector_nums);
//       free(dirents);
//       free(inode);
//       return -1;
//     }
//     for (long unsigned int j = 0; j < DISKIMG_SECTOR_SIZE/sizeof(uint16_t); j++) {
//       if (diskimg_readsector(fs->dfd, sector_nums_2[j], dirents) == -1) {
//         free(sector_nums_2);
//         free(sector_nums);
//         free(dirents);
//         free(inode);
//         return -1;
//       }
//       for (long unsigned int k = 0; k < DISKIMG_SECTOR_SIZE/sizeof(struct direntv6); k++) {
//         if (strcmp(dirents[k].d_name, name) == 0) {
//           printf("3\n");
//           *dirEnt = dirents[k];
//           free(sector_nums_2);
//           free(sector_nums);
//           free(dirents);
//           free(inode);
//           return 0;
//         }
//       }
//     }
//   }
//   free(sector_nums_2);
//   free(sector_nums);
//   free(dirents);
//   free(inode);
//   dirEnt = NULL;
//   return -2;
// }

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
        // Small directory
        for (int i = 0; i < block_count; i++) {
            if (diskimg_readsector(fs->dfd, inode->i_addr[i], dirents) == -1) {
                free(dirents);
                free(inode);
                return -1;
            }
            for (int j = 0; j < DISKIMG_SECTOR_SIZE / sizeof(struct direntv6); j++) {
                if (strcmp(dirents[j].d_name, name) == 0) {
                    *dirEnt = dirents[j];
                    free(dirents);
                    free(inode);
                    return 0;
                }
            }
        }
    } else {
        // Large directory
        uint16_t* sector_nums = (uint16_t*) malloc(DISKIMG_SECTOR_SIZE);
        if (sector_nums == NULL) {
            free(dirents);
            free(inode);
            return -1;
        }

        for (int i = 0; i < 7; i++) {
            if (diskimg_readsector(fs->dfd, inode->i_addr[i], sector_nums) == -1) {
                free(sector_nums);
                free(dirents);
                free(inode);
                return -1;
            }
            for (int j = 0; j < DISKIMG_SECTOR_SIZE / sizeof(uint16_t); j++) {
                if (diskimg_readsector(fs->dfd, sector_nums[j], dirents) == -1) {
                    free(sector_nums);
                    free(dirents);
                    free(inode);
                    return -1;
                }
                for (int k = 0; k < DISKIMG_SECTOR_SIZE / sizeof(struct direntv6); k++) {
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

        // Double indirect case
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

        for (int i = 0; i < DISKIMG_SECTOR_SIZE / sizeof(uint16_t); i++) {
            if (diskimg_readsector(fs->dfd, sector_nums[i], sector_nums_2) == -1) {
                free(sector_nums_2);
                free(sector_nums);
                free(dirents);
                free(inode);
                return -1;
            }
            for (int j = 0; j < DISKIMG_SECTOR_SIZE / sizeof(uint16_t); j++) {
                if (diskimg_readsector(fs->dfd, sector_nums_2[j], dirents) == -1) {
                    free(sector_nums_2);
                    free(sector_nums);
                    free(dirents);
                    free(inode);
                    return -1;
                }
                for (int k = 0; k < DISKIMG_SECTOR_SIZE / sizeof(struct direntv6); k++) {
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
    return -2;  // Name not found
}