
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
    if (fs == NULL || pathname == NULL) return -1;
    int count;
    const char* delimiter = "/";
    char** split_strings = split(pathname, delimiter, &count);

    int dirinumber = 0;
    for (int i = 0; i < count; i++) {
        struct direntv6 dirent;
        printf("initial inumber: %i", dirent.d_inumber);
        if (directory_findname(fs, split_strings[i], dirinumber, &dirent) == -1) {
            for (int j = 0; j < count; j++) {
                free(split_strings[i]);
            }
            free(split_strings);
            return -1;
        }
        dirinumber = dirent.d_inumber;
    }
    for (int i = 0; i < count; i++) {
        free(split_strings[i]);
    }
    free(split_strings);
	return dirinumber;
}

// Bug encontrado: directory_findname está metiendo en dirent un dirent cuyo inumber es 0. 