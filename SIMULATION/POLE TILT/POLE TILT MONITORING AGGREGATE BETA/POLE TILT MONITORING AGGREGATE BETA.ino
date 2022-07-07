#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define DEBUG 1

LiquidCrystal_I2C lcd(0x27,16,2);

RF24 radio(10,9); //CE, CSN

uint8_t address[][6] = {"1Node", "2Node"};
bool radioNumber = 0; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
bool role = false;  // true = TX role, false = RX role

int upload_counter = 0;
const int axis = A2;
const float max_angle = 10, agg_def_angle = 333;

float agg_angle = 0, node_angle = 0, payload = 0.0;

void truncate(String* str, int columns) {
  *str = str->substring(0, columns);
}
void print_lcd(String str, int row) {
  static const int COLUMNS = 16;
  if (str.length() > COLUMNS) truncate(&str, COLUMNS);
  int pre_spaces = (COLUMNS - str.length()) / 2;
  lcd.setCursor(0, row); lcd.print("                ");
  lcd.setCursor(pre_spaces, row); lcd.print(str);
}
void print_lcd(String str0, String str1) {
  static const int COLUMNS = 16;
  if (str0.length() > COLUMNS) truncate(&str0, COLUMNS);
  if (str1.length() > COLUMNS) truncate(&str1, COLUMNS);
  int pre_spaces0 = (COLUMNS - str0.length()) / 2;
  int pre_spaces1 = (COLUMNS - str1.length()) / 2;
  lcd.clear();
  lcd.setCursor(pre_spaces0, 0); lcd.print(str0);
  lcd.setCursor(pre_spaces1, 1); lcd.print(str1);
}
void default_lcd_text() {
  print_lcd("IoT pole","tilt monitoring");
}
float measure_tilt_angle() {
  const int iterations = 10;
  float acc_angle = 0;
  for(int i = 0; i < iterations; i++) {
    acc_angle += abs(analogRead(axis));
    delay(100);
  }
  // if(DEBUG) Serial.println(acc_angle / iterations); //265, 398
  float difference = abs((acc_angle / iterations) - agg_def_angle);
  // if(DEBUG) Serial.println(difference * (90.0 / 65.0));
  return abs(difference * (90.0 / 65.0));
}
bool receive_data(){
  unsigned long t = millis();
  while(millis() - t < 2000) {
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&payload, bytes);            // fetch payload from FIFO
      node_angle = payload;
      if(node_angle < 0) node_angle = 0.0;
      if(DEBUG) {
      //   Serial.print(F("Received "));
        // Serial.print(bytes);                    // print the size of the payload
      //   Serial.print(F(" bytes on pipe "));
      //   Serial.print(pipe);                     // print the pipe number
        // Serial.print(F(": "));
        Serial.println(payload);                // print the payload's value
        Serial.println();
      }
      return true;
    }
    return false;
  }
}
void upload_to_server() {
  print_lcd("uploading","data");
  if(Serial.available()) Serial.readString();
  delay(1500);
  String data = "https://victechelectronics.000webhostapp.com/pole/api/insert.php?agregate=" + (String)agg_angle + "&node=" + (String)node_angle;
  Serial.println(data);
  unsigned long t = millis();
  while(millis() - t < 8000 & !Serial.available());
  if(Serial.available()){
    String response = Serial.readString();
    if(response.indexOf("SUCCESS") != -1) print_lcd("data sent","successfully");
    else print_lcd("couldnt send","data");
  }
  else print_lcd("WiFi module","not responding");
  delay(1500);
}
void sleep() {
  print_lcd("entering","sleep mode");
  delay(1700);
  lcd.noBacklight();
  lcd.noDisplay();
  delay(5000);
  lcd.backlight();
  lcd.display();
}
void setup() {
  Serial.begin(9600);
  // pinMode(axis, INPUT);
  if(DEBUG) {
    if (!radio.begin()) Serial.println(("radio hardware is not responding!!"));
    else Serial.println("hardware OK");
  }
  else radio.begin();
  radio.setPALevel(RF24_PA_MAX);  // RF24_PA_MAX is default.
  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes
  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0
  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1
  radio.startListening(); // put radio in RX mode
  lcd.init();
  lcd.backlight();
  default_lcd_text();
  delay(2000);
}
void loop() {
  // print_lcd("checking for","incoming data");
  // agg_angle = measure_tilt_angle();
  bool data_received = receive_data();
  // if(data_received) {
  //   print_lcd("agg angle: " + (String)agg_angle, "node angle:" + (String)node_angle);
  //   if(DEBUG) Serial.println("agg angle: " + (String)agg_angle + "  node angle:" + (String)node_angle);
  // }
  // else {
  //   print_lcd("agg angle: " + (String)agg_angle, "node angle: N/A");
  //   if(DEBUG) Serial.println("agg angle: " + (String)agg_angle + "  node angle: N/A");
  // }
  // delay(1500);
  // String row0_status, row1_status;
  // if(agg_angle < max_angle) row0_status = "agg pole is okay";
  // else row0_status = "agg too tilted";
  // if(data_received) {
  //   if(node_angle < max_angle) row1_status = "node pole is ok";
  //   else row1_status = "node too tilted";
  // }
  // else row1_status = "node data N/A";
  // print_lcd(row0_status, row1_status);
  // delay(800);
  // upload_to_server();
  // if(upload_counter == 2) {
  //   upload_to_server();
  //   upload_counter = 0;
  // }
  // else upload_counter++;
  // sleep();
}
