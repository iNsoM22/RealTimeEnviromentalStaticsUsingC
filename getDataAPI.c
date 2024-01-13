#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include "timeUNIXConversion.h"
#include "storeResponse.h"

#define MAX_LOCATION_SIZE 20
#define MAX_URL_SIZE 256
#define BASE_ADDRESS "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/"
#define API_KEY "GSF87S3N5CL3FLVMY4264S9RU"


/**
 * @brief Perform an HTTP GET request to Visual Crossing API, retrieve data, and save it to a file.
 *
 * This function constructs the URL based on the provided location and date,
 * performs an HTTP GET request using cURL, retrieves the data, saves the JSON response to a file,
 * and frees allocated memory.
 *
 * @param location Pointer to the location string.
 * @param date Pointer to the date string.
 * @return 0 on success, -1 on failure.
 */

int get_data(char **location, char **date){
	CURL *curl;
	CURLcode result;
	char URL[MAX_URL_SIZE];
	int success;

	// Construct the URL for the API request
	snprintf(URL, MAX_URL_SIZE, "%s%s/%s?key=%s", BASE_ADDRESS, *location, *date, API_KEY);

	// Ensures the memory is free
	free(*location);
	free(*date); 

	// Initialize cURL
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	
	// Check if cURL initialization failed
	if(curl == NULL){
		fprintf(stderr, "Curl Library Initialization Failure.");
		return -1;
	}

	// Initialize the response structure
	Response response;
    response.string = NULL;
    response.size = 0;

	// Set cURL options
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_chunk);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

	// Perform the cURL request
	result = curl_easy_perform(curl);
	
	// Check for cURL request failure
	if(result != CURLE_OK){
		fprintf(stderr, "Error : %s\n", curl_easy_strerror(result));
		return -1;
	}

	printf("%s\n", response.string);

	// cURL lib cleanup
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	// Save JSON response to a file
	success = save_json_to_file("output.json", response.string);
	free(response.string);

	if (success != 0) {
        fprintf(stderr, "Error: Failed to save JSON to file.\n");
    }

	return 0;
}


int main(){
	printf("Getting Data\n");
	
	// Allocate memory for location	
	char *location;
	location = malloc(MAX_LOCATION_SIZE*sizeof(char));

	if(location == NULL){
		fprintf(stderr, "Memory Allocation Failed.");
		return -1;
	}
	printf("Enter the location: ");
	fgets(location, 20, stdin);
	location[strcspn(location, "\n")] = '\0';

	// Allocate memory for timestamp
    char *timestamp = dateUNIXConversion();
	if(timestamp == NULL){
		fprintf(stderr, "Error: Invalid Timestamp.");
		return -1;
	}

	get_data(&location, &timestamp);
	
	return 0;
}

