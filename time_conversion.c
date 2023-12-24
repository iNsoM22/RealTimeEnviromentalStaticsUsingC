#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT_SIZE 20

int dateUNIXConversion() {
    char input[MAX_INPUT_SIZE];
    struct tm tm;
    time_t t;

    // Allocate memory for date string
    char *date = (char *)malloc(MAX_INPUT_SIZE * sizeof(char));
    if (date == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return 1;
    }

    printf("Enter the date (YYYY/MM/DD HH:MM:SS): ");
    
    // Read input using fgets
    fgets(input, sizeof(input), stdin);
    if (input == NULL) {
        fprintf(stderr, "Error reading input.\n");
        free(date);  // Free allocated memory before exiting
        return 1;
    }

    // Copy input to dynamically allocated memory
    strncpy(date, input, MAX_INPUT_SIZE);
    date[MAX_INPUT_SIZE - 1] = '\0';  // Ensure null-termination

    // Remove newline character if present
    date[strcspn(date, "\n")] = '\0';

    strptime(date, "%Y/%m/%d %H:%M:%S", &tm);
    free(date);

    if (&tm == NULL) {
        fprintf(stderr, "Invalid Date and Time Format.\n");
        return 1;
    }


    t = mktime(&tm);
    printf("Date and time in Unix epoch format: %ld\n", (long)t);
    return (long)t;
}
