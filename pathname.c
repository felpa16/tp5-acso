#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


char **split(const char *str, const char *delimiter, int *count) {
    // Copy the input string to avoid modifying the original string
    char *str_copy = strdup(str);
    if (str_copy == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    // Count the number of tokens
    int token_count = 0;
    char *temp = strdup(str);
    if (temp == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    int len = strlen(str_copy);
    str_copy[len-1] = '\0';
    char *token = strtok(temp, delimiter);
    while (token != NULL) {
        token_count++;
        token = strtok(NULL, delimiter);
    }
    free(temp);

    // Allocate memory for the array of strings
    char **result = malloc((token_count + 1) * sizeof(char *));
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Split the string and store the tokens in the array
    int index = 0;
    int len = strlen(str_copy);
    str_copy[len-1] = '\0';
    token = strtok(str_copy, delimiter);
    while (token != NULL) {
        result[index] = strdup(token);
        if (result[index] == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        index++;
        token = strtok(NULL, delimiter);
    }
    result[index] = NULL; // Null-terminate the array

    // Set the count of tokens
    *count = token_count;

    // Free the copied string
    free(str_copy);

    return result;
}

/**
 * TODO
 */
int pathname_lookup(struct unixfilesystem *fs, const char *pathname) {
    
    printf("Se llama a pathname_lookup con path %s\n", pathname);
    if (fs == NULL || pathname == NULL || strlen(pathname) == 0 || pathname[0] != '/') return -1;
    const char* home = "/";
    if (strcmp(pathname, home) == 0){
        return 1;
    }
    char* path_copy = strdup(pathname);
    char* token = strtok(path_copy, "/");
    int count;
    const char* delimiter = "/";
    char** split_strings = split(pathname, delimiter, &count);
    if (count == 0) {
        for (int i = 0; i < count; i++) {
            free(split_strings[i]);
        }
        free(split_strings);
        printf("There are no directories in the specified path (count = 0)\n");
        return -1;
    }

    int dirinumber = ROOT_INUMBER;
    for (int i = 0; i < count; i++) {
        struct direntv6 dirent;
        if (directory_findname(fs, split_strings[i], dirinumber, &dirent) == -1) { // esta fallando aca
            for (int j = 0; j < count; j++) {
                free(split_strings[i]);
            }
            free(split_strings);
            printf("Falla directory_findname\n");
            return -1;
        }
        dirinumber = dirent.d_inumber;
        struct inode buf;
        if (inode_iget(fs, dirinumber, &buf) == -1) {
            for (int j = 0; j < count; j++) {
                free(split_strings[i]);
            }
            free(split_strings);
            printf("Falla el inode_iget con dirinumber\n");
            return -1;
        }
    }
    for (int i = 0; i < count; i++) {
        free(split_strings[i]);
    }
    free(split_strings);
	return dirinumber;
}


















// #include "pathname.h"
// #include "directory.h"
// #include "inode.h"
// #include "diskimg.h"
// #include <stdio.h>
// #include <string.h>
// #include <assert.h>
// #include <stdlib.h>


// char **split(const char *str, const char *delimiter, int *count) {
//     // Copy the input string to avoid modifying the original string
//     char *str_copy = strdup(str);
//     if (str_copy == NULL) {
//         perror("strdup");
//         exit(EXIT_FAILURE);
//     }

//     // Count the number of tokens
//     int token_count = 0;
//     char *temp = strdup(str);
//     if (temp == NULL) {
//         perror("strdup");
//         exit(EXIT_FAILURE);
//     }
//     char *token = strtok(temp, delimiter);
//     while (token != NULL) {
//         token_count++;
//         token = strtok(NULL, delimiter);
//     }
//     free(temp);

//     // Allocate memory for the array of strings
//     char **result = malloc((token_count + 1) * sizeof(char *));
//     if (result == NULL) {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }

//     // Split the string and store the tokens in the array
//     int index = 0;
//     token = strtok(str_copy, delimiter);
//     while (token != NULL) {
//         result[index] = strdup(token);
//         if (result[index] == NULL) {
//             perror("strdup");
//             exit(EXIT_FAILURE);
//         }
//         index++;
//         token = strtok(NULL, delimiter);
//     }
//     result[index] = NULL; // Null-terminate the array

//     // Set the count of tokens
//     *count = token_count;

//     // Free the copied string
//     free(str_copy);

//     return result;
// }

// /**
//  * TODO
//  */
// int pathname_lookup(struct unixfilesystem *fs, const char *pathname) {
//     printf("Se llama a pathname_lookup con path %s\n", pathname);
//     if (fs == NULL || pathname == NULL) return -1;
//     const char* home = "/";
//     if (strcmp(pathname, home) == 0){
//         return 1;
//     }
//     int dirinumber;
//     int count;
//     const char* delimiter = "/";
//     char** split_strings = split(pathname, delimiter, &count);
//     if (count == 0) {
//         for (int i = 0; i < count; i++) {
//             free(split_strings[i]);
//         }
//         free(split_strings);
//         printf("There are no directories in the specified path (count = 0)\n");
//         return -1;
//     }
//     for (int i = 0; i < count; i++) {
//         for (int d = 0; d < fs->superblock.s_ninode; d++) {
//             struct direntv6 dirent;
//             int result = directory_findname(fs, split_strings[i], d, &dirent);
//             if (result == -1) { // la funcion directory_findname falla
//                 for (int j = 0; j < count; j++) {
//                     free(split_strings[i]);
//                 }
//                 free(split_strings);
//                 printf("Falla directory_findname\n");
//                 return -1;
//             }
//             else if (result == -2) { //la funcion directory_findname no encuentra el directorio que le pasamos
//                 continue;
//             }
//             // la funcion directory_findname encontró el directorio que le pasamos
//             dirinumber = dirent.d_inumber;
//         }
//     }

//     for (int i = 0; i < count; i++) {
//         free(split_strings[i]);
//     }
//     free(split_strings);
// 	return dirinumber;
// }

// // Bug encontrado: directory_findname está metiendo en dirent un dirent cuyo inumber es 0. 
// // Bug profundizdo: la razon por al cual pathname_lookup está devolviendo 0 es porque no entra al for.