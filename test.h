#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Receives a string and a divisor. Divides the string into smaller strings separated
by the divisor and returns an array of said strings.
*/
char **split(const char *str, const char *delimiter, int *count);