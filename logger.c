#include "logger.h"

void record_module_call()
{
    // Record the current date and time
    time_t currentTime;
    time(&currentTime);

    // Convert the time to a struct tm in local time
    struct tm *timeInfo = localtime(&currentTime);

    // Format the date and time as "YYYY/MM/DD HH:MM:SS"
    char formattedTime[20];
    strftime(formattedTime, sizeof(formattedTime), "%Y/%m/%d %H:%M:%S", timeInfo);

    // Open a file to store the last call date
    FILE *file = fopen("functionCalls.log", "w");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    // Write the formatted date and time to the file
    if (fprintf(file, "%s", formattedTime) < 0)
    {
        perror("Error writing to file");
    }

    // Close the file
    fclose(file);
}

// Function to check if 7 days have passed since the last call
int record_checker()
{
    FILE *file = fopen("functionCalls.log", "r");
    if (!file)
    {
        // File doesn't exist or couldn't be opened
        perror("Error opening file");
        return 1; // Return true to make the call when the file doesn't exist or an error occurs
    }

    char lastCallDateStr[20]; // Adjust the size as needed

    // Read the last call date with the correct format
    if (fscanf(file, "%19[^\n]", lastCallDateStr) == EOF)
    {
        if (feof(file))
        {
            fprintf(stderr, "Error: Logs are empty.\n");
        }
        else
        {
            fprintf(stderr, "Error reading the last call date from logs.\n");
        }

        fclose(file);
        return 1; // Return 1 on error
    }

    // Close the file
    fclose(file);

    char *date = date_unix_conversion(lastCallDateStr);
    if (date == NULL)
    {
        fprintf(stderr, "Error converting date to UNIX format.\n");
        return 1;
    }

    long lastdate = atoi(date);

    // Get the current date and time
    time_t currentTime;
    time(&currentTime);

    // Free the dynamically allocated memory for date
    free(date);

    // Check if 7 days (604800 seconds) have passed
    return (lastdate == -1 || (currentTime - lastdate) >= 604800) ? 1 : 0;
}
