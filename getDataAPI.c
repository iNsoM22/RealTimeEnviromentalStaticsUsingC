#include "getDataAPI.h"

int get_data(const char *rawFile, const char *location, const char *date, const char *BASE_ADDRESS, const char *API_KEY)
{
    CURL *curl;
    CURLcode result;
    char *URL = malloc(MAX_URL_SIZE);
    int success;

    // Construct the URL for the API request
    snprintf(URL, MAX_URL_SIZE, "%s%s/%s?key=%s", BASE_ADDRESS, location, date, API_KEY);

    // Initialize cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    // Check if cURL initialization failed
    if (curl == NULL)
    {
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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

    // Perform the cURL request
    result = curl_easy_perform(curl);

    // Check for cURL request failure
    if (result != CURLE_OK)
    {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
        return -1;
    }

    // cURL lib cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // Save JSON response to a file
    success = save_json_to_file(rawFile, response.string);

    // Free allocated memory for the response string
    free(response.string);
    free(URL);

    if (success != 0)
    {
        fprintf(stderr, "Error: Failed to save JSON to file.\n");
        return -1;
    }

    return 0;
}
