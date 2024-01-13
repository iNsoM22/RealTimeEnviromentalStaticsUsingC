#include "weatherProcessing.h"

int isDuplicate(cJSON *existingResultArray, cJSON *newDayDict) {
    int array_size = cJSON_GetArraySize(existingResultArray);

    // Iterate through the existing 'result' array
    for (int i = 0; i < array_size; i++) {
        cJSON *existingDayDict = cJSON_GetArrayItem(existingResultArray, i);

        // Compare datetime values
        cJSON *existingDatetime = cJSON_GetObjectItemCaseSensitive(existingDayDict, "datetime");
        cJSON *newDatetime = cJSON_GetObjectItemCaseSensitive(newDayDict, "datetime");

        if (cJSON_IsString(existingDatetime) && cJSON_IsString(newDatetime) &&
            (strcmp(existingDatetime->valuestring, newDatetime->valuestring) == 0)) {
            // Duplicate entry found
            return 1;  // True
        }
    }

    // No duplicate entry found
    return 0;  // False
}

int retrieveAndWriteData(char *raw_data_filename, char *processed_data_file) {
    FILE *raw_file = fopen(raw_data_filename, "r");
    if (!raw_file) {
        perror("Error opening input file");
        return 1;  // Return 1 on failure
    }

    fseek(raw_file, 0, SEEK_END);
    long fileSize = ftell(raw_file);
    fseek(raw_file, 0, SEEK_SET);

    if (fileSize <= 0) {
        printf("ERROR: Empty file or file size is not valid.\n");
        fclose(raw_file);
        return 1;  // Return 1 on failure
    }

    char *raw_file_data = (char *)malloc(fileSize + 1);
    if (!raw_file_data) {
        printf("ERROR: Memory allocation failed.\n");
        fclose(raw_file);
        return 1;  // Return 1 on failure
    }

    size_t len = fread(raw_file_data, 1, fileSize, raw_file);
    if (len != (size_t)fileSize) {
        printf("ERROR: Unable to read the entire file.\n");
        fclose(raw_file);
        free(raw_file_data);
        return 1;  // Return 1 on failure
    }

    // Null-terminate the raw_file_data
    raw_file_data[len] = '\0';

    cJSON *json = cJSON_Parse(raw_file_data);
    free(raw_file_data);
    fclose(raw_file);

    if (!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) {
            printf("ERROR: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    cJSON *daysArray = cJSON_GetObjectItemCaseSensitive(json, "days");
    if (!cJSON_IsArray(daysArray)) {
        printf("ERROR: 'days' is not an array in the JSON.\n");
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    cJSON *resultArray = cJSON_CreateArray();
    int array_size = cJSON_GetArraySize(daysArray);

    // Iterate through the 'days' array
    for (int i = 0; i < array_size; i++) {
        cJSON *day_item = cJSON_GetArrayItem(daysArray, i);
        if (!cJSON_IsObject(day_item)) {
            printf("ERROR: Element at index %d is not an object.\n", i);
            continue;
        }

        cJSON *datetime = cJSON_GetObjectItemCaseSensitive(day_item, "datetime");
        cJSON *tempmax = cJSON_GetObjectItemCaseSensitive(day_item, "tempmax");
        cJSON *tempmin = cJSON_GetObjectItemCaseSensitive(day_item, "tempmin");
        cJSON *temp = cJSON_GetObjectItemCaseSensitive(day_item, "temp");
        cJSON *humidity = cJSON_GetObjectItemCaseSensitive(day_item, "humidity");
        cJSON *description = cJSON_GetObjectItemCaseSensitive(day_item, "description");

        // Create a new dictionary for each day
        cJSON *dayDict = cJSON_CreateObject();
        cJSON_AddItemToObject(dayDict, "datetime", cJSON_Duplicate(datetime, 1));
        cJSON_AddItemToObject(dayDict, "tempmax", cJSON_Duplicate(tempmax, 1));
        cJSON_AddItemToObject(dayDict, "tempmin", cJSON_Duplicate(tempmin, 1));
        cJSON_AddItemToObject(dayDict, "temp", cJSON_Duplicate(temp, 1));
        cJSON_AddItemToObject(dayDict, "humidity", cJSON_Duplicate(humidity, 1));
        cJSON_AddItemToObject(dayDict, "description", cJSON_Duplicate(description, 1));

        // Add the day dictionary to the 'result' array
        cJSON_AddItemToArray(resultArray, dayDict);
    }

    // Read the existing content of 'processed_output.json'
    FILE *processed_file = fopen(processed_data_file, "r");
    if (!processed_file) {
        printf("ERROR: Unable to open %s for reading.\n", processed_data_file);
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    fseek(processed_file, 0, SEEK_END);
    long existingFileSize = ftell(processed_file);
    fseek(processed_file, 0, SEEK_SET);

    char *existingFileData = (char *)malloc(existingFileSize + 1);
    if (!existingFileData) {
        printf("ERROR: Memory allocation failed.\n");
        fclose(processed_file);
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    size_t existingFileLen = fread(existingFileData, 1, existingFileSize, processed_file);
    if (existingFileLen != (size_t)existingFileSize) {
        printf("ERROR: Unable to read the existing file.\n");
        fclose(processed_file);
        free(existingFileData);
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    existingFileData[existingFileLen] = '\0';

    cJSON *existingJson = cJSON_Parse(existingFileData);
    free(existingFileData);
    fclose(processed_file);

    if (!existingJson) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) {
            printf("ERROR: %s\n", error_ptr);
        }
        cJSON_Delete(existingJson);
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    cJSON *existingResultArray = cJSON_GetObjectItemCaseSensitive(existingJson, "result");

    // Append the new data to the existing 'result' array
    if (!cJSON_IsArray(existingResultArray)) {
        printf("ERROR: 'result' is not an array in the existing JSON.\n");
        cJSON_Delete(existingJson);
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    for (int i = 0; i < cJSON_GetArraySize(resultArray); i++) {
        cJSON *dayDict = cJSON_GetArrayItem(resultArray, i);

        if (!isDuplicate(existingResultArray, dayDict)) {
            // Add the day dictionary to the 'existingResultArray' only if it's not a duplicate
            cJSON_AddItemToArray(existingResultArray, cJSON_Duplicate(dayDict, 1));
        }
    }

    // Write the modified data
    FILE *output_file = fopen(processed_data_file, "w");
    if (!output_file) {
        perror("Error opening output file");
        cJSON_Delete(existingJson);
        cJSON_Delete(json);
        return 1;  // Return 1 on failure
    }

    char *outputJsonString = cJSON_Print(existingJson);
    fprintf(output_file, "%s", outputJsonString);

    fclose(output_file);
    cJSON_free(outputJsonString);
    cJSON_Delete(existingJson);
    cJSON_Delete(json);

    return 0;  // Return 0 on success
}
