#ifndef WEATHER_PROCESSING_H
#define WEATHER_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

int retrieveAndWriteData(char *raw_data_filename, char *processed_data_file);

#endif // WEATHERPROCESSING_H
