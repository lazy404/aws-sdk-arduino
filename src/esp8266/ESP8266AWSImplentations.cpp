#include <stdio.h>
/* application.h is Esp8266's standard library. Defines the Arduino String
 * object, the Arduino delay() procedure, and the Esp8266 TCPClient. */
#include "Esp8266AWSImplementations.h"
#include "DeviceIndependentInterfaces.h"
#include <ESP8266WiFi.h>
#include <string.h>

int delayTime = 500;
char* updateCurTime(void);

Esp8266HttpClient::Esp8266HttpClient() {
}

char* Esp8266HttpClient::send(const char* request, const char* serverUrl, int port) {

    // WiFiClientSecure client;
    Serial.println(serverUrl);
    Serial.println(port);
    Serial.println(request);
    Serial.println("");
    Serial.println("");

    /* Arduino String to build the response with. */
    String responseBuilder = "Response: ";
    if (client.connect(serverUrl, port)) {
        /* Send the requests */
        client.print(request);
        client.print("\n");
        /* Read the request into responseBuilder. */
        delay(delayTime);
        Serial.println("<");
        while (client.available()) {
            char c = client.read();
            responseBuilder.concat(c);
            Serial.print(".");
        }
        Serial.println(">");
        client.stop();
    } else {
        client.stop();
        /* Error connecting. */
        return "can't setup SSL connection";
    }
    /* Copy responseBuilder into char* */
    int len = responseBuilder.length();
    char* response = new char[len + 1]();
    responseBuilder.toCharArray(response, len + 1);
    return response;
}

bool Esp8266HttpClient::usesCurl() {
    /* Does not use curl command. */
    return false;
}

Esp8266DateTimeProvider::Esp8266DateTimeProvider() {
    /* No need to sync, spark sychronizes time on startup. */
    //strcpy(dateTime, updateCurTime2(client2));
}

const char* Esp8266DateTimeProvider::getDateTime() {
    // return "20151224120100";
    return updateCurTime();
}
bool Esp8266DateTimeProvider::syncTakesArg(void) {
    return true;
}

void Esp8266DateTimeProvider::sync(const char* dateTime) {
  // should have no need for an implementation
}

////////////////////////////////////
// convert month to digits
////////////////////////////////////
String getMonth(String sM) {
  if(sM=="Jan") return "01";
  if(sM=="Feb") return "02";
  if(sM=="Mar") return "03";
  if(sM=="Apr") return "04";
  if(sM=="May") return "05";
  if(sM=="Jun") return "06";
  if(sM=="Jul") return "07";
  if(sM=="Aug") return "08";
  if(sM=="Sep") return "09";
  if(sM=="Oct") return "10";
  if(sM=="Nov") return "11";
  if(sM=="Dec") return "12";
  return "01";
}

////////////////////////////////////
// Scrape UTC Time from server
////////////////////////////////////
char* updateCurTime(void) {
    static int timeout_busy=0;
    int ipos;
    timeout_busy=0; //reset

    const char* timeServer = "aws.amazon.com";

    // send a bad header on purpose, so we get a 400 with a DATE: timestamp
    const char* timeServerGet = "GET example.com/ HTTP/1.1";
    String utctime;
    String GmtDate;
    static char dateStamp[20];
    static char chBuf[200];
    char utctimeraw[80];
    char* dpos;

    WiFiClient client2;
    if (client2.connect(timeServer, 80)) {
        //Send Request
        client2.println(timeServerGet);
        client2.println();
        while((!client2.available())&&(timeout_busy++<5000)){
          // Wait until the client sends some data
          delay(1);
        }

        // kill client if timeout
        if(timeout_busy>=5000) {
            client2.flush();
            client2.stop();
            Serial.println("timeout receiving timeserver data\n");
            return dateStamp;
        }

        // read the http GET Response
        String req2 = client2.readString();
        // Serial.println("");
        // Serial.println("");
        // Serial.print(req2);
        // Serial.println("");
        // Serial.println("");

        // close connection
        delay(1);
        client2.flush();
        client2.stop();

        ipos = req2.indexOf("Date:");
        if(ipos>0) {
          GmtDate = req2.substring(ipos,ipos+35);
          // Serial.println(GmtDate);
          utctime = GmtDate.substring(18,22) + getMonth(GmtDate.substring(14,17)) + GmtDate.substring(11,13) + GmtDate.substring(23,25) + GmtDate.substring(26,28) + GmtDate.substring(29,31);
          // Serial.println(utctime.substring(0,14));
          utctime.substring(0,14).toCharArray(dateStamp, 20);
        }
    }
    else {
      Serial.println("did not connect to timeserver\n");
    }
    timeout_busy=0;     // reset timeout
    return dateStamp;   // Return latest or last good dateStamp
}
