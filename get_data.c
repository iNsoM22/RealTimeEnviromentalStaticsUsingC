#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>

#define BASE_ADDRESS "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/"
#define API_KEY "GSF87S3N5CL3FLVMY4264S9RU"

int get_data(char **location, char **date){
	CURL *curl;
	CURLcode result;
	char URL[128];

	strcat(URL, BASE_ADDRESS);
	strcat(URL, *location);
	strcat(URL, "/");
	strcat(URL, *date);
	strcat(URL, "?key=");
	strcat(URL, API_KEY);

	free(*location);
	free(*date); 

	curl = curl_easy_init();
	if(curl == NULL){
		fprintf(stderr, "Curl Library Initialization Failure.");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, URL);
	result = curl_easy_perform(curl);
	if(result != CURLE_OK){
		fprintf(stderr, "Error : %s\n", curl_easy_strerror(result));
		return -1;
	}

	printf("%u", result);
	curl_easy_cleanup(curl);
	return 0;
}


int main(){
	printf("Getting Data\n");
	char *location, *date;
	location = malloc(20*sizeof(char));
	date = malloc(20*sizeof(char));
	if(location == NULL || date == NULL){
		fprintf(stderr, "Memory Allocation Failed.");
		return -1;
	}
	printf("Enter the location: ");
	scanf("%s", location);
	printf("Enter the timestamp: ");
	scanf("%s", date);

	return get_data(&location, &date);
}
