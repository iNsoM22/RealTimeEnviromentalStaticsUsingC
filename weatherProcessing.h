/**
 * @file weatherProcessing.h
 *
 * @brief Functions for processing weather data.
 */

#ifndef WEATHERPROCESSING_H
#define WEATHERPROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

/**
 * @brief Check if a day's data is a duplicate in the existing array.
 *
 * This function checks if a day's data (given by the newDayDict) is already present
 * in the existing 'result' array.
 *
 * @param existingResultArray The existing 'result' array containing days' data.
 * @param newDayDict The dictionary representing a day's data to check for duplication.
 * @return 1 if the day is a duplicate, 0 otherwise.
 */
int is_duplicate(cJSON *existingResultArray, cJSON *newDayDict);

/**
 * @brief Retrieve and write processed weather data to a file.
 *
 * This function reads raw weather data from a file, processes it, and appends the
 * processed data to an existing file. It checks for duplicates before adding new data.
 *
 * @param raw_data_filename Name of the file containing raw weather data.
 * @param processed_data_file Name of the file to store the processed weather data.
 * @return 0 on success, 1 on failure.
 */
int retrieve_and_write_data(char *raw_data_filename, char *processed_data_file);

#endif // WEATHERPROCESSING_H
