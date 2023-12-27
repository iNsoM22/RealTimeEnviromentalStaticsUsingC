#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT_SIZE 20


/**
 * @brief Convert a timestamp to Unix time.
 *
 * This function prompts the user to enter a timestamp in the format "YYYY/MM/DD HH:MM:SS"
 * or specify "last7days" or "last30days". It then converts the timestamp to Unix time.
 * If successful, it returns the Unix time as a string; otherwise, it returns NULL.
 *
 * @return A string representing Unix time or NULL on failure.
 */


char *dateUNIXConversion() {
    struct tm tm;
    time_t t;

    // Allocate memory for date string
    char *date = (char *)malloc(MAX_INPUT_SIZE * sizeof(char));

    // Checks if the Memory Allocation was successfull or not.
    if (date == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    printf("Enter the TimeStamp: ");
    
    // Read input using fgets
    fgets(date, MAX_INPUT_SIZE, stdin);

    // Checks if the User has entered the input or not.
    if (date == NULL) {
        fprintf(stderr, "Error reading input.\n");
        free(date); 
        return NULL;
    }

    // Removes the newline character if present.
    date[strcspn(date, "\n")] = '\0';

    // Checks if the date is dynamic or specific.
    if(!strcmp(date, "last7days") || !strcmp(date, "last30days")){
        return date;
    }

    // Converts the string-formatted date into time structure.
    strptime(date, "%Y/%m/%d %H:%M:%S", &tm);

    // Checks if the date is successfully converted into the time structure.
    if (&tm == NULL) {
        fprintf(stderr, "Invalid Date and Time Format.\n");
        return NULL;
    }

    // Converts the time-structured date into seconds representation. 
    t = mktime(&tm);

    // Allocate memory for the string representation of Unix time.
    char *result = (char *)malloc(20 * sizeof(char));
    
    if (result == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    // Convert Unix time to string
    sprintf(result, "%ld", (long)t);
    
    free(date);

    return result;
}