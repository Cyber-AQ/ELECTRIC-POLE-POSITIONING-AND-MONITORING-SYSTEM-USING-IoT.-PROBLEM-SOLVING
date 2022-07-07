#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define DEBUG 1

RF24 radio(10,9); //CE, CSN

uint8_t address[][6] = {"1Node", "2Node"};
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
bool role = true;  // true = TX role, false = RX role
float payload = 0.0, def_node_angle = 329;

const int axis = A5, led = 2;

float measure_tilt_angle() {
  const int iterations = 10;
  float acc_angle = 0;
  for(int i = 0; i < iterations; i++) {
    acc_angle += abs(analogRead(axis));
    delay(100);
  }
  // if(DEBUG) Serial.println(acc_angle / iterations); //265, 398
  float difference = abs((acc_angle / iterations) - def_node_angle);
  // if(DEBUG) Serial.println(difference * (90.0 / 65.0));
  return abs(difference * (90.0 / 65.0));
}
void transmit_to_aggregate(float angle) {
  unsigned long start_timer = micros();                    // start the timer
  bool report = radio.write(&angle, sizeof(float));      // transmit & save the report
  unsigned long end_timer = micros();                      // end the timer
  if(DEBUG) { 
    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
    //   Serial.print(F("Time to transmit = "));
    //   Serial.print(end_timer - start_timer);                 // print the timer result
    //   Serial.print(F(" us. Sent: "));
    //   Serial.println(payload);                               // print payload sent
    //   payload += 0.01;                                       // increment float payload
    Serial.println();
    }
    else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }
    // delay(1000);  // slow transmissions down by 1 second
  }
}
void blink_led(int blink_times, int on_delay) {
  digitalWrite(led, 1);
  delay(on_delay);
  digitalWrite(led,0);
  delay(1000 - on_delay);
}
void setup() {
  if(DEBUG) Serial.begin(9600);
  pinMode(axis, INPUT);
  pinMode(led, OUTPUT);
  if(!radio.begin()) Serial.println("hardware failed");
  else Serial.println("hardware OK");
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes
  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0
  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1
  // additional setup specific to the node's role
  radio.stopListening();  // put radio in TX mode
}
void loop() {
  float tilt_angle = measure_tilt_angle();
  // float tilt_angle = abs(def_node_angle - measure_tilt_angle()) * (90.0 / 66.0);
  // if(DEBUG) Serial.println(tilt_angle);
  transmit_to_aggregate(tilt_angle);
  // blink_led(1,50);
}
