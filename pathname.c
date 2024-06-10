#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "string_split.h"

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