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