/**
   BasicHTTPSClient.ino

    Created on: 20.08.2018

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
// const uint8_t fingerprint[20] = {0x40, 0xaf, 0x00, 0x6b, 0xec, 0x90, 0x22, 0x41, 0x8e, 0xa3, 0xad, 0xfa, 0x1a, 0xe8, 0x25, 0x41, 0x1d, 0x1a, 0x54, 0xb3};
const uint8_t fingerprint[20] = {0xF3, 0x1B, 0xB7, 0x47, 0x29, 0x59, 0x39, 0xC1, 0x91, 0x7D, 0xB4, 0x61, 0xDA, 0x4D, 0xEC, 0x0D, 0x8C, 0xE1, 0xE7, 0xC1};
// F3:1B:B7:47:29:59:39:C1:91:7D:B4:61:DA:4D:EC:0D:8C:E1:E7:C1
ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(9600);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Nokia 3.1", "lenovo1234");
}

void loop() {
  // wait for WiFi connection
  if(Serial.available()) {
    String str = Serial.readStringUntil('\r');
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      client->setFingerprint(fingerprint);
      HTTPClient https;
      // Serial.print("[HTTPS] begin...\n");
      // if (https.begin(*client, "https://jigsaw.w3.org/HTTP/connection.html")) {
      // if (https.begin(*client, "https://victechelectronics.000webhostapp.com/pole/api/insert.php?agregate=6&node=3")) {
      if (https.begin(*client, str)) {
        // Serial.print("[HTTPS] GET...\n");+
        int httpCode = https.GET();
        if (httpCode > 0) {
          // Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }
    else Serial.println("no connected to hotspot");
    while(Serial.available()) Serial.read();
  }
}
