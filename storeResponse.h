// response_handler.h

#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Response structure
typedef struct {
    char *string;
    size_t size;
} Response;

// Declare the write_chunk function
size_t write_chunk(void *data, size_t size, size_t nmemb, void *userdata);

// Declare the save_json_to_file function
int save_json_to_file(const char *filename, const char *json);

#endif  // RESPONSE_HANDLER_H
