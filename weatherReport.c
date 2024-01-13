#include "weatherReport.h"

void calculate_averages(struct WeatherData data[], int size, float *avgTempMax, float *avgTempMin, float *avgTemp, float *avgHumidity)
{
    float sumTempMax = 0, sumTempMin = 0, sumTemp = 0, sumHumidity = 0;

    for (int i = 0; i < size; i++)
    {
        sumTempMax += data[i].tempmax;
        sumTempMin += data[i].tempmin;
        sumTemp += data[i].temp;
        sumHumidity += data[i].humidity;
    }

    *avgTempMax = sumTempMax / size;
    *avgTempMin = sumTempMin / size;
    *avgTemp = sumTemp / size;
    *avgHumidity = sumHumidity / size;
}

int read_json_from_file(const char *filename, struct WeatherData **data, int *size)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return -1;
    }

    // Read file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer for JSON data
    char *json_buffer = (char *)malloc(file_size + 1);
    if (!json_buffer)
    {
        perror("Memory allocation failed for JSON buffer");
        fclose(file);
        return -1;
    }

    // Read JSON data into the buffer
    fread(json_buffer, 1, file_size, file);
    fclose(file);

    json_buffer[file_size] = '\0';

    // Parse JSON data
    cJSON *root = cJSON_Parse(json_buffer);
    free(json_buffer);

    if (!root)
    {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    // Extract 'result' array from JSON
    cJSON *result = cJSON_GetObjectItem(root, "result");
    *size = cJSON_GetArraySize(result);

    // Allocate memory for WeatherData array
    *data = (struct WeatherData *)malloc(*size * sizeof(struct WeatherData));
    if (!*data)
    {
        perror("Memory allocation failed for WeatherData array");
        cJSON_Delete(root);
        return -1;
    }

    // Populate WeatherData array from JSON
    for (int i = 0; i < *size; i++)
    {
        cJSON *item = cJSON_GetArrayItem(result, i);
        sscanf(cJSON_GetObjectItem(item, "datetime")->valuestring, "%s", (*data)[i].datetime);
        (*data)[i].tempmax = cJSON_GetObjectItem(item, "tempmax")->valuedouble;
        (*data)[i].tempmin = cJSON_GetObjectItem(item, "tempmin")->valuedouble;
        (*data)[i].temp = cJSON_GetObjectItem(item, "temp")->valuedouble;
        (*data)[i].humidity = cJSON_GetObjectItem(item, "humidity")->valuedouble;

        cJSON *descriptionItem = cJSON_GetObjectItem(item, "description");
        if (cJSON_IsString(descriptionItem))
        {
            strncpy((*data)[i].description, descriptionItem->valuestring, sizeof((*data)[i].description) - 1);
            (*data)[i].description[sizeof((*data)[i].description) - 1] = '\0';
        }
        else
        {
            (*data)[i].description[0] = '\0';
        }
    }

    cJSON_Delete(root);

    return 0;
}

void generate_PDF(const char *filename, struct WeatherData data[], int dataSize, float avgTempMax, float avgTempMin, float avgTemp, float avgHumidity)
{
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_PageDirection direction;

    // Create PDF document
    pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        fprintf(stderr, "Error: Unable to create PDF document\n");
        return;
    }

    // Add a page to the PDF
    page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, direction);

    // Set font and font size
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);

    // Write header
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, font, 20);
    HPDF_Page_TextOut(page, 170, HPDF_Page_GetHeight(page) - 70, "Weather Abnormality Report");
    HPDF_Page_SetFontAndSize(page, font, 11);
    HPDF_Page_TextOut(page, 50, HPDF_Page_GetHeight(page) - 100, " Date        TempMax    TempMin    Temp     Humidity     Abnormality                   Description");
    HPDF_Page_EndText(page);
    font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 10);

    // Write weather data
    for (int i = 0; i < dataSize; i++)
    {
        HPDF_Page_BeginText(page);
        char buffer[250];
        if ((data[i].temp < avgTemp) && (data[i].humidity < avgHumidity))
        {
            sprintf(buffer, "%-14s  %-14.1f  %-17.1f  %-12.1f  %-14.1f  %-16s  %-10s",
                    data[i].datetime, data[i].tempmax, data[i].tempmin, data[i].temp, data[i].humidity, "      None           ", data[i].description);
        }
        else
        {
            sprintf(buffer, "%-14s  %-14.1f  %-17.1f  %-12.1f  %-14.1f  %-16s  %-10s",
                    data[i].datetime, data[i].tempmax, data[i].tempmin, data[i].temp, data[i].humidity, data[i].temp > avgTemp ? "Temperature" : "   Humidity   ", data[i].description);
        }
        HPDF_Page_TextOut(page, 50, HPDF_Page_GetHeight(page) - 120 - i * 15, buffer);
        HPDF_Page_EndText(page);
    }

    // Write averages
    HPDF_Page_BeginText(page);
    char avgBuffer[100];
    font = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
    HPDF_Page_SetFontAndSize(page, font, 16);
    HPDF_Page_TextOut(page, 50, 70, "Averages: ");
    font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 11);
    sprintf(avgBuffer, "TempMax: %.2f     TempMin: %.2f     Temp: %.2f     Humidity: %.2f\n", avgTempMax, avgTempMin, avgTemp, avgHumidity);
    HPDF_Page_TextOut(page, 50, 50, avgBuffer);

    HPDF_Page_EndText(page);

    // Save the PDF to file
    HPDF_SaveToFile(pdf, filename);

    // Clean up
    HPDF_Free(pdf);
}

int create_report(char *processedFilename, char *pdfFilename)
{
    // JSON data
    struct WeatherData *data = NULL;

    int dataSize;
    if (read_json_from_file(processedFilename, &data, &dataSize) != 0)
    {
        return 1; // Exit with error code
    }

    // Calculate averages
    float avgTempMax, avgTempMin, avgTemp, avgHumidity;
    calculate_averages(data, dataSize, &avgTempMax, &avgTempMin, &avgTemp, &avgHumidity);

    // Generate PDF
    generate_PDF(pdfFilename, data, dataSize, avgTempMax, avgTempMin, avgTemp, avgHumidity);
    printf("PDF generated successfully.\n");

    // Free allocated memory for WeatherData
    free(data);

    return 0;
}
