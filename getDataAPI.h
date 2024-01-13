/**
 * @file getDataAPI.h
 *
 * @brief Functions for retrieving weather data from an API.
 */

#ifndef GET_DATA_API_H
#define GET_DATA_API_H

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "storeResponse.h"

#define MAX_URL_SIZE 256

/**
 * @brief Retrieves weather data from the specified API and saves it to a file.
 *
 * This function constructs a URL based on the provided location and date, performs a cURL request,
 * and saves the received JSON response to a file.
 *
 * @param rawFile Path to the file to save the raw JSON response.
 * @param location The location for which to retrieve weather data.
 * @param date The date for which to retrieve weather data.
 * @param BASE_ADDRESS The base address of the weather API.
 * @param API_KEY The API key for authentication.
 * @return 0 on success, -1 on failure.
 */
int get_data(const char *rawFile, const char *location, const char *date, const char *BASE_ADDRESS, const char *API_KEY);

#endif
