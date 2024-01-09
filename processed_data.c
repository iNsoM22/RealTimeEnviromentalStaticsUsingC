#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

void retrieveAndWriteData() {
    FILE *raw_file = fopen("output.json", "r");
    if (raw_file == NULL) {
        perror("Error opening input file");
        return;
    }

    fseek(raw_file, 0, SEEK_END);
    long fileSize = ftell(raw_file);
    fseek(raw_file, 0, SEEK_SET);

    if (fileSize <= 0) {
        printf("ERROR: Empty file or file size is not valid.\n");
        fclose(raw_file);
        return;
    }

    char *raw_file_data = (char *)malloc(fileSize + 1);
    if (raw_file_data == NULL) {
        printf("ERROR: Memory allocation failed.\n");
        fclose(raw_file);
        return;
    }

    size_t len = fread(raw_file_data, 1, fileSize, raw_file);
    if (len != (size_t)fileSize) {
        printf("ERROR: Unable to read the entire file.\n");
        fclose(raw_file);
        free(raw_file_data);
        return;
    }

    // Null-terminate the raw_file_data
    raw_file_data[len] = '\0';

    cJSON *json = cJSON_Parse(raw_file_data);
    free(raw_file_data);

    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("ERROR: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        fclose(raw_file);
        return;
    }

    cJSON *daysArray = cJSON_GetObjectItemCaseSensitive(json, "days");
    if (cJSON_IsArray(daysArray)) {
        cJSON *resultArray = cJSON_CreateArray();

        int array_size = cJSON_GetArraySize(daysArray);

        // Iterate through the 'days' array
        for (int i = 0; i < array_size; i++) {
            cJSON *day_item = cJSON_GetArrayItem(daysArray, i);
            if (cJSON_IsObject(day_item)) {
                cJSON *datetime = cJSON_GetObjectItemCaseSensitive(day_item, "datetime");
                cJSON *tempmax = cJSON_GetObjectItemCaseSensitive(day_item, "tempmax");
                cJSON *tempmin = cJSON_GetObjectItemCaseSensitive(day_item, "tempmin");
                cJSON *temp = cJSON_GetObjectItemCaseSensitive(day_item, "temp");

                // Create a new dictionary for each day
                cJSON *dayDict = cJSON_CreateObject();
                cJSON_AddItemToObject(dayDict, "datetime", cJSON_Duplicate(datetime, 1));
                cJSON_AddItemToObject(dayDict, "tempmax", cJSON_Duplicate(tempmax, 1));
                cJSON_AddItemToObject(dayDict, "tempmin", cJSON_Duplicate(tempmin, 1));
                cJSON_AddItemToObject(dayDict, "temp", cJSON_Duplicate(temp, 1));

                // Add the day dictionary to the 'result' array
                cJSON_AddItemToArray(resultArray, dayDict);
            } else {
                printf("ERROR: Element at index %d is not an object.\n", i);
            }
        }

        // Read the existing content of 'processed_output.json'
        FILE *processed_file = fopen("processed_output.json", "r");
        if (processed_file != NULL) {
            fseek(processed_file, 0, SEEK_END);
            long existingFileSize = ftell(processed_file);
            fseek(processed_file, 0, SEEK_SET);

            char *existingFileData = (char *)malloc(existingFileSize + 1);
            if (existingFileData != NULL) {
                size_t existingFileLen = fread(existingFileData, 1, existingFileSize, processed_file);
                if (existingFileLen == (size_t)existingFileSize) {
                    existingFileData[existingFileLen] = '\0';

                    cJSON *existingJson = cJSON_Parse(existingFileData);
                    free(existingFileData);

                    if (existingJson != NULL) {
                        cJSON *existingResultArray = cJSON_GetObjectItemCaseSensitive(existingJson, "result");

                        // Append the new data to the existing 'result' array
                        if (cJSON_IsArray(existingResultArray)) {
                            for (int i = 0; i < cJSON_GetArraySize(resultArray); i++) {
                                cJSON *dayDict = cJSON_GetArrayItem(resultArray, i);
                                cJSON_AddItemToArray(existingResultArray, cJSON_Duplicate(dayDict, 1));
                            }

                            // Write the modified data to 'processed_output.json'
                            FILE *output_file = fopen("processed_output.json", "w");
                            if (output_file == NULL) {
                                perror("Error opening output file");
                                cJSON_Delete(json);
                                cJSON_Delete(existingJson);
                                fclose(processed_file);
                                return;
                            }

                            char *outputJsonString = cJSON_Print(existingJson);
                            fprintf(output_file, "%s", outputJsonString);

                            fclose(output_file);
                            cJSON_free(outputJsonString);
                            cJSON_Delete(existingJson);

                        } else {
                            printf("ERROR: 'result' is not an array in the existing JSON.\n");
                            cJSON_Delete(existingJson);
                        }
                    } else {
                        const char *error_ptr = cJSON_GetErrorPtr();
                        if (error_ptr != NULL) {
                            printf("ERROR: %s\n", error_ptr);
                        }
                        cJSON_Delete(existingJson);
                    }
                } else {
                    printf("ERROR: Unable to read the existing file.\n");
                }

                fclose(processed_file);
            } else {
                printf("ERROR: Memory allocation failed.\n");
            }
        } else {
            printf("ERROR: Unable to open processed_output.json for reading.\n");
        }
    } else {
        printf("ERROR: 'days' is not an array in the JSON.\n");
    }

    cJSON_Delete(json);
    fclose(raw_file);
}

int main() {
    retrieveAndWriteData();
    return 0;
}
