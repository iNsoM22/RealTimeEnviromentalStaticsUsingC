#include"storeResponse.h"



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

size_t write_chunk(void *data, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;

    // Cast userdata to Response structure
	Response *response = (Response *) userdata;

    // Reallocate memory for the response string
	char *ptr = realloc(response->string, response->size + total_size + 1);
	if(ptr == NULL){
		fprintf(stderr, "Memory Allocation Failed.");
		return 0; // Return 0 to indicate failure.
	}

    // Update the response structure with the new string and size   
	response->string = ptr;

    // Copies the block of memory (data) to the response.
	memcpy(&(response->string[response->size]), data, total_size);
	response->size += total_size;

    // Ensure null-termination.
	response->string[response->size] = '\0';

    return total_size;
}



/**
 * @brief Save JSON string to a file.
 *
 * This function takes a filename and a JSON string, and writes the JSON string to the specified file.
 *
 * @param filename Name of the file to save the JSON to.
 * @param json The JSON string to be saved.
 * @return 0 on success, -1 on failure.
 */

int save_json_to_file(const char *filename, const char *json) {
    FILE *file = fopen(filename, "w");

    // Checks if the file is opened succesfully or not.
    if (file == NULL) {
        fprintf(stderr, "Error opening file");
        return -1; // Return -1 to indicate failure
    }
    
    // Writes the string data to the file.
    fprintf(file, "%s", json);
    
    // Closes the file.
    fclose(file);

    return 0;
}