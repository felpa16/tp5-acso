
#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "test.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
