#include "timeUNIXConversion.h"

char *date_unix_conversion(const char *formattedDate)
{
    struct tm tm;
    time_t t;

    // Checks if the User has entered the input or not.
    if (formattedDate == NULL)
    {
        fprintf(stderr, "Error: NULL input.\n");
        return NULL;
    }

    // Allocate memory for the string representation of Unix time.
    char *result = (char *)malloc(20);
    if (result == NULL)
    {
        perror("Memory allocation error");
        return NULL;
    }

    // Checks if the date is dynamic or specific.
    if (strcmp(formattedDate, "last7days") == 0 || strcmp(formattedDate, "last30days") == 0 ||
        strcmp(formattedDate, "next7days") == 0 || strcmp(formattedDate, "next30days") == 0)
    {
        strcpy(result, formattedDate);
        return result;
    }

    // Removes the newline character if present.
    char *dateCopy = strdup(formattedDate);
    if (dateCopy == NULL)
    {
        perror("Memory allocation error");
        free(result);
        return NULL;
    }

    dateCopy[strcspn(dateCopy, "\n")] = '\0';

    // Converts the string-formatted date into a time structure.
    if (strptime(dateCopy, "%Y/%m/%d %H:%M:%S", &tm) == NULL)
    {
        fprintf(stderr, "Error: Invalid Date and Time Format.\n");
        free(result);
        free(dateCopy);
        return NULL;
    }

    free(dateCopy);

    // Converts the time-structured date into seconds representation.
    t = mktime(&tm);

    // Convert Unix time to string
    snprintf(result, 20, "%ld", (long)t);

    return result;
}
