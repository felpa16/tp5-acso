#include "test.h"

// Function to split a string into an array of strings based on a delimiter
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

// int main() {
//     const char *str = "/hello world/i like/meat";
//     const char *delimiter = "/";
//     int count;
//     char **result = split(str, delimiter, &count);

//     // Print the result
//     printf("Number of tokens: %d\n", count);
//     for (int i = 0; i < count; i++) {
//         printf("Token %d: %s\n", i, result[i]);
//         free(result[i]); // Free each token
//     }
//     free(result); // Free the array

//     return 0;
// }