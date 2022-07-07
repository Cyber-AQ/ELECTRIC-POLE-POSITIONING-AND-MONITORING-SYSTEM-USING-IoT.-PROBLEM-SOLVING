#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial ss(6,7);
LiquidCrystal lcd(8,9,10,11,12,13);

int tilt_sensor = A0, tilt_limit = 920;

void concatenate(String* str, int columns) {
  *str = str->substring(0, columns);
}
void print_lcd(String str, int row) {
  static const int COLUMNS = 16;
  if (str.length() > COLUMNS) concatenate(&str, COLUMNS);
  int pre_spaces = (COLUMNS - str.length()) / 2;
  lcd.setCursor(0, row); lcd.print("                ");
  lcd.setCursor(pre_spaces, row); lcd.print(str);
}
void print_lcd(String str0, String str1) {
  static const int COLUMNS = 16;
  if (str0.length() > COLUMNS) concatenate(&str0, COLUMNS);
  if (str1.length() > COLUMNS) concatenate(&str1, COLUMNS);
  int pre_spaces0 = (COLUMNS - str0.length()) / 2;
  int pre_spaces1 = (COLUMNS - str1.length()) / 2;
  lcd.clear();
  lcd.setCursor(pre_spaces0, 0); lcd.print(str0);
  lcd.setCursor(pre_spaces1, 1); lcd.print(str1);
}
void default_lcd_text(){
  print_lcd("IoT POLE","TILT DETECTION");
}
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  pinMode(tilt_sensor, INPUT);
  lcd.begin(16,2);
  default_lcd_text();
  delay(1000);
}

void loop() {
  int sensor_input = analogRead(tilt_sensor);
  if(sensor_input < tilt_limit) {
    ss.println("NODE B: too much tilt!");
    print_lcd("warning!","too much tilt");
  }
  else {
    ss.println("NODE B: pole safe");
    print_lcd("safe","pole ok");
  }
  if(Serial.available()) {
    String data = Serial.readStringUntil('\r');
    ss.println(data);
  }
  delay(1000);
}
