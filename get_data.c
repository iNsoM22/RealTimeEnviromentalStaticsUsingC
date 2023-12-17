#include<stdio.h>
#include<curl/curl.h>

int main(void){
	CURL *curl;
	CURLcode result;
	curl = curl_easy_init();
	if(curl == NULL){
		fprintf(stderr, "CUrl Lib Error");
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts");
	result = curl_easy_perform(curl);
	if(result != CURLE_OK){
		fprintf(stderr, "Error : %s\n", curl_easy_strerror(result));
		return -1;
	}
	printf("%u", result);
	curl_easy_cleanup(curl);
	return 0;
}
