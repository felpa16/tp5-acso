#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "string_split.h"

char **split(const char *str, const char *delimiter, int *count) {

    char *str_copy = strdup(str);
    int token_count = 0;
    char *temp = strdup(str);
    char *token = strtok(temp, delimiter);

    while (token != NULL) {
        token_count++;
        token = strtok(NULL, delimiter);
    }
    free(temp);

    char **result = malloc((token_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    int index = 0;
    token = strtok(str_copy, delimiter);
    while (token != NULL) {
        result[index] = strdup(token);
        if (result[index] == NULL) {
            return NULL;
        }
        index++;
        token = strtok(NULL, delimiter);
    }
    result[index] = NULL;
    *count = token_count;
    free(str_copy);
    return result;
}

int pathname_lookup(struct unixfilesystem *fs, const char *pathname) {

    if (fs == NULL || pathname == NULL) return -1;
    if (strcmp(pathname, "/") == 0) {
        return 1;
    }

    int dirinumber = 1;
    int count;
    const char* delimiter = "/";
    char** split_strings = split(pathname, delimiter, &count);

    if (count == 0) {
        printf("There are no directories in the specified path (count = 0)\n");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        struct direntv6 dirent;
        int result = directory_findname(fs, split_strings[i], dirinumber, &dirent);
        if (result != 0) {
            for (int j = 0; j < count; j++) {
                free(split_strings[j]);
            }
            free(split_strings);
            printf("Failed to find directory: %s\n", split_strings[i]);
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