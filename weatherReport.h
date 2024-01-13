#ifndef WEATHER_REPORT_H
#define WEATHER_REPORT_H

/**
 * @file weatherReport.h
 * @brief Header file for weather report functionality.
 */

/**
 * @struct WeatherData
 * @brief Structure to hold weather data information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <hpdf.h>

struct WeatherData
{
    char datetime[20];     ///< Date and time of the weather data.
    float tempmax;         ///< Maximum temperature.
    float tempmin;         ///< Minimum temperature.
    float temp;            ///< Temperature.
    float humidity;        ///< Humidity.
    char description[100]; ///< Weather description.
};

/**
 * @brief Calculate average values for temperature and humidity from an array of WeatherData.
 *
 * @param data Array of WeatherData structures.
 * @param size Size of the array.
 * @param avgTempMax Pointer to store the average maximum temperature.
 * @param avgTempMin Pointer to store the average minimum temperature.
 * @param avgTemp Pointer to store the average temperature.
 * @param avgHumidity Pointer to store the average humidity.
 */
void calculate_averages(struct WeatherData data[], int size, float *avgTempMax, float *avgTempMin, float *avgTemp, float *avgHumidity);

/**
 * @brief Read JSON data from a file and populate an array of WeatherData structures.
 *
 * @param filename File path to the JSON file.
 * @param data Pointer to the array of WeatherData structures.
 * @param size Pointer to store the size of the array.
 * @return 0 on success, -1 on failure.
 */
int read_json_from_file(const char *filename, struct WeatherData **data, int *size);

/**
 * @brief Generate a PDF report with weather data and average values.
 *
 * @param filename File path to save the PDF report.
 * @param data Array of WeatherData structures.
 * @param dataSize Size of the array.
 * @param avgTempMax Average maximum temperature.
 * @param avgTempMin Average minimum temperature.
 * @param avgTemp Average temperature.
 * @param avgHumidity Average humidity.
 */
void generate_PDF(const char *filename, struct WeatherData data[], int dataSize, float avgTempMax, float avgTempMin, float avgTemp, float avgHumidity);

/**
 * @brief Create a weather report PDF based on processed JSON data.
 *
 * @param processedFilename File path to the processed JSON data.
 * @param pdfFilename File path to save the generated PDF report.
 * @return 0 on success, 1 on failure.
 */
int create_report(char *processedFilename, char *pdfFilename);

#endif /* WEATHER_REPORT_H */
