#include "ClientJson.h"

int ClientJson::sendRequest(const char * type, JsonObject& json)
{
    // connect to server
    if(!connect()) {
        return returnError(HTTPC_ERROR_CONNECTION_REFUSED);
    }

    if(json.measureLength() > 0) {
        addHeader(F("Content-Length"), String(json.measureLength()));
    }

    // send Header
    if(!sendHeader(type)) {
        return returnError(HTTPC_ERROR_SEND_HEADER_FAILED);
    }

    json.printTo(*_tcp);

    // handle Server Response (Header)
    return returnError(handleHeaderResponse());
}

int ClientJson::sendRequest(const char * type, JsonArray& json)
{
    // connect to server
    if(!connect()) {
        return returnError(HTTPC_ERROR_CONNECTION_REFUSED);
    }

    if(json.measureLength() > 0) {
        addHeader(F("Content-Length"), String(json.measureLength()));
    }

    // send Header
    if(!sendHeader(type)) {
        return returnError(HTTPC_ERROR_SEND_HEADER_FAILED);
    }

    json.printTo(*_tcp);

    // handle Server Response (Header)
    return returnError(handleHeaderResponse());
}
