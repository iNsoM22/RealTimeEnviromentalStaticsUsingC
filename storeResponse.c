#include "storeResponse.h"

size_t write_chunk(void *data, size_t size, size_t nmemb, void *userdata)
{
    size_t total_size = size * nmemb;

    // Cast userdata to Response structure
    Response *response = (Response *)userdata;

    // Reallocate memory for the response string
    char *ptr = realloc(response->string, response->size + total_size + 1);
    if (ptr == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.");
        return 0; // Return 0 to indicate failure.
    }

    // Update the response structure with the new string and size
    response->string = ptr;

    // Copies the block of memory (data) to the response.
    memcpy(&(response->string[response->size]), data, total_size);
    response->size += total_size;

    // Ensure null-termination.
    response->string[response->size] = '\0';

    return total_size;
}

int save_json_to_file(const char *filename, const char *json)
{
    FILE *file = fopen(filename, "w");

    // Checks if the file is opened successfully or not.
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file");
        return -1; // Return -1 to indicate failure
    }

    // Writes the string data to the file.
    size_t bytes_written = fwrite(json, sizeof(char), strlen(json), file);

    // Closes the file.
    fclose(file);

    // Check if the write was successful.
    if (bytes_written != strlen(json))
    {
        fprintf(stderr, "Error writing to file");
        return -1; // Return -1 to indicate failure
    }

    return 0;
}
