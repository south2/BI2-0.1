/*************************************************************
  bodyinteraction.org
  sample program for raeding MPU data, setting LED color and motor speed
*/
#define BLYNK_PRINT Serial
// include this library in the Arduino library manager
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1
// LED data pin is connected to pin?
#define DATA_PIN 14

// Define the array of leds
CRGB leds[NUM_LEDS];
int wave;

// include this library in the Arduino library manager
#include <MPU9250_asukiaaa.h>
MPU9250 mySensor;
float aX, aY, aZ, mDirection, pitch, roll, yaw;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "XXXXXXXXXXXXX";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YYYYYYYYYYYYY";
char pass[] = "ZZZZZZZZZZZZZ";

BlynkTimer timer;

void myTimerEvent()
{
  // read acceleration data
  mySensor.accelUpdate();
  aX = mySensor.accelX();
  aY = mySensor.accelY();
  aZ = mySensor.accelZ();
  // read gyroscope update
  mySensor.magUpdate();
  mDirection = mySensor.magHorizDirection();
  // calculate pitch, roll, yaw (raw approximation)
  float pitch = 180 * atan (aX/sqrt(aY*aY + aZ*aZ))/M_PI;
  float roll = 180 * atan (aY/sqrt(aX*aX + aZ*aZ))/M_PI;
  float yaw = 180 * atan (aZ/sqrt(aX*aX + aZ*aZ))/M_PI;
  // send data to app via virtual ports, e.g. virtual pin V2 is set to pitch
  Blynk.virtualWrite(V2, pitch);
  Blynk.virtualWrite(V3, roll);
  Blynk.virtualWrite(V4, yaw);
  Blynk.virtualWrite(V5, mDirection);
}

BLYNK_WRITE(V0) // set RGB color values which are transmitted from the app as V0 (virtual pin 0)
{   
  int i = param[0].asInt();
  int j = param[1].asInt();
  int k = param[2].asInt();
  leds[0].setRGB(j,i,k);
  FastLED.show();
}

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  Wire.begin(4, 5); //sda, scl
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginMag();

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}

