/**
 * @file storeResponse.h
 *
 * @brief Functions for handling HTTP responses and storing data.
 */

#ifndef STORE_RESPONSE_H
#define STORE_RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct Response
 * @brief Structure to store HTTP response data.
 */
typedef struct
{
    char *string; /**< Pointer to the received data. */
    size_t size;  /**< Size of the data. */
} Response;

/**
 * @brief Write received data chunk to response buffer.
 *
 * This function is used as the CURLOPT_WRITEFUNCTION when performing a curl request.
 * It writes the received data chunk to the response buffer, reallocating memory as needed.
 *
 * @param data Pointer to the received data.
 * @param size Size of each data element.
 * @param nmemb Number of elements.
 * @param userdata Pointer to the Response structure.
 * @return Total size of the data chunk.
 */
size_t write_chunk(void *data, size_t size, size_t nmemb, void *userdata);

/**
 * @brief Save JSON string to a file.
 *
 * This function takes a filename and a JSON string, and writes the JSON string to the specified file.
 *
 * @param filename Name of the file to save the JSON to.
 * @param json The JSON string to be saved.
 * @return 0 on success, -1 on failure.
 */
int save_json_to_file(const char *filename, const char *json);

#endif
