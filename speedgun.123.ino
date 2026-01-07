#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SENSOR_1_PIN 2 
#define SENSOR_2_PIN 3 

volatile unsigned long timeSensor1 = 0;
volatile unsigned long timeSensor2 = 0;
volatile bool sensor1Triggered = false;
volatile bool sensor2Triggered = false;
const float distanceBetweenSensors = 0.1; 
float speed = 0; 

void sensor1ISR() {
 if (!sensor1Triggered) 
 { 
   timeSensor1 = micros(); 
   sensor1Triggered = true;
 }
}

void sensor2ISR() {
 if (!sensor2Triggered)
  { 
   timeSensor2 = micros(); 
   sensor2Triggered = true;
  }
}

void setup() {
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0, 0);
 lcd.print("Speed Gun");
 delay(2000);
 lcd.clear();
 pinMode(SENSOR_1_PIN, INPUT);
 pinMode(SENSOR_2_PIN, INPUT);
 attachInterrupt(digitalPinToInterrupt(SENSOR_1_PIN), sensor1ISR, RISING);
 attachInterrupt(digitalPinToInterrupt(SENSOR_2_PIN), sensor2ISR, RISING);
}


void loop() {
 if (sensor1Triggered && sensor2Triggered) {
   sensor1Triggered = false;
   sensor2Triggered = false;
   unsigned long timeDifference = abs(timeSensor2 - timeSensor1); 
   lcd.clear();
   if (timeDifference > 0) {
     speed = distanceBetweenSensors / (timeDifference / 1000000.0); s
     lcd.setCursor(0, 0);
     lcd.print("Speed:");
     lcd.setCursor(0, 1);
     lcd.print(speed, 2); 
     lcd.print(" m/s");
   } else {
     lcd.setCursor(0, 0);
     lcd.print("Invalid Reading!");
   }
 }
}