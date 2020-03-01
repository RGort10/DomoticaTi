#include "domoticaTi.h"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
   return size * nmemb;
}

int pingArduino(char* staticip) {
  int response;
  CURL *curl = curl_easy_init();
  if(curl) {
    CURLcode res;
    char url[200];
    sprintf(url, "%s/", staticip);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 500L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    res = curl_easy_perform(curl);
    if(res == CURLE_OK) {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
      response = (int)response_code;
    } else {
      response = -1;
    }
    curl_easy_cleanup(curl);
  }

  return response;
}

int pingArduinoComponent(char* staticip, char* arduinoComponentId) {
  int response;
  CURL *curl = curl_easy_init();
  if(curl) {
    CURLcode res;
    char url[200];
    sprintf(url, "%s/%s/ping", staticip, arduinoComponentId);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 500L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    res = curl_easy_perform(curl);
    if(res == CURLE_OK) {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
      response = (int)response_code;
    } else {
      response = -1;
    }
    curl_easy_cleanup(curl);
  }

  return response;
}