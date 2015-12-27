#include "AmazonIOTClient.h"
#include "AWSFoundationalTypes.h"
#include <stdlib.h>
#include "Utils.h"

AmazonIOTClient::AmazonIOTClient() : AWSClient4() {
    this->awsService = "iotdata";
    this->contentType = "application/json";
    this->signedHeaders = "host;x-amz-content-sha256;x-amz-date";
    this->uri = "/";
    this->queryString = "";
    this->httpS = true;
}

char* AmazonIOTClient::update_shadow(MinimalString shadow, ActionError& actionError) {
    actionError = NONE_ACTIONERROR;

    this->method = "POST";
    char* request = createRequest(shadow);
    char* response = sendData(request);
    return response;


    // delete[] request;
    //
    // if (response == NULL) {
    //     actionError = CONNECTION_ACTIONERROR;
    //     return 999;
    // }
    //
    // int httpStatusCode = findHttpStatusCode(response);
    //
    // if (httpStatusCode == 200) {
    //   return 1;
    // }
    //
    // if (httpStatusCode == 403) {
    //     char* ts = strstr(response, "earlier than ");
    //     int pos = ts - response;
    //
    //     char* newts = new char[IOT_EXTRACTED_TIMESTAMP_BUFFER_LENGTH]();
    //     strncpy(newts, response + pos + 31, IOT_EXTRACTED_TIMESTAMP_BUFFER_LENGTH - 1);
    //     newts[16] = '\0';
    //
    //     char* time = new char[IOT_FORMATTED_TIMESTAMP_BUFFER_LENGTH]();
    //     sprintf(time, "%.8s%.6s", newts, newts + 9);
    //     dateTimeProvider->sync(time);
    //     return 0;
    // }
    //
    // return httpStatusCode;
}
