#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

int hourglassSum(int arr_rows, int arr_columns, int** arr) {
    int max_sum = INT_MIN; // Initialize to the smallest possible integer

    for (int i = 0; i <= 3; i++) {
        for (int j = 0; j <= 3; j++) {
            int top = arr[i][j] + arr[i][j + 1] + arr[i][j + 2];
            int middle = arr[i + 1][j + 1];
            int bottom = arr[i + 2][j] + arr[i + 2][j + 1] + arr[i + 2][j + 2];
            int hourglass_sum = top + middle + bottom;

            if (hourglass_sum > max_sum) {
                max_sum = hourglass_sum;
            }
        }
    }

    return max_sum;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
    if (fptr == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    int** arr = malloc(6 * sizeof(int*));
    if (arr == NULL) {
        perror("Failed to allocate memory for array");
        fclose(fptr);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 6; i++) {
        arr[i] = malloc(6 * sizeof(int));
        if (arr[i] == NULL) {
            perror("Failed to allocate memory for row");
            for (int k = 0; k < i; k++) {
                free(arr[k]);
            }
            free(arr);
            fclose(fptr);
            return EXIT_FAILURE;
        }

        char** arr_item_temp = split_string(rtrim(readline()));
        for (int j = 0; j < 6; j++) {
            int arr_item = parse_int(arr_item_temp[j]);
            arr[i][j] = arr_item;
        }
        free(arr_item_temp); // Free split string array
    }

    int result = hourglassSum(6, 6, arr);
    fprintf(fptr, "%d\n", result);

    // Clean up
    for (int i = 0; i < 6; i++) {
        free(arr[i]);
    }
    free(arr);
    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);
    if (data == NULL) {
        return NULL; // Handle malloc failure
    }

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) {
            break;
        }
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }
        alloc_length <<= 1;
        char* temp = realloc(data, alloc_length);
        if (temp == NULL) {
            free(data); // Free original data on failure
            return NULL;
        }
        data = temp;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
        if (data == NULL) {
            return NULL;
        }
    } else {
        data = realloc(data, data_length + 1);
        if (data == NULL) {
            return NULL;
        }
        data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return NULL;
    while (*str != '\0' && isspace(*str)) {
        str++;
    }
    return str;
}

char* rtrim(char* str) {
    if (!str) return NULL;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;

    while (token) {
        char** temp = realloc(splits, sizeof(char*) * ++spaces);
        if (temp == NULL) {
            free(splits);
            return NULL; // Handle memory allocation failure
        }
        splits = temp;
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }
    return value;
}
