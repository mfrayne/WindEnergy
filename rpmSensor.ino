#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Define the pin connections
const int irSensorPin = 7;  // IR sensor output pin connected to digital pin 7
const int ledPin = 13;      // LED connected to digital pin 13 (optional)
float rotations = 0;
unsigned long previousTime = 0; 
const unsigned long interval = 2000;

//OLED screen variables
int WHITE = 1;//pixel on
int BLACK = 0;//pixel off
int SCREEN_WIDTH = 128;  // OLED display width, in pixels
int SCREEN_HEIGHT = 64;  // OLED display height, in pixels
// Declaration for SSD1306 display connected using I2C
int SCREEN_ADDRESS = 0x3C;
//-1 default reset code for screen (same as arduino)
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  pinMode(irSensorPin, INPUT);  // Set IR sensor pin as input
  Serial.begin(9600);           // Begin serial communication for debugging
  if (!display.begin(SCREEN_ADDRESS, true)) {
    Serial.println(F("SSD allocation failed"));
    while(true){}//loops forever if allocation of display fails 
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  int sensorValue = digitalRead(irSensorPin);  // Read the value from the IR sensor

  if (sensorValue == LOW) {
    rotations++;
    delay(1);
  } 

  unsigned long timePassed = millis()-previousTime;

  if(timePassed >=interval){
    //updateDisplay();
    //float minutesPassed = timePassed/60000.0;
    float rpm = rotations*(60000.0/timePassed);
    updateDisplay(rpm);
    Serial.println(rpm);
    //Serial.println(minutesPassed);
    rotations = 0;
    previousTime = millis();
  }

  delay(1);  // Small delay for stability
}

void updateDisplay(float rpm){
  //clears display buffer so data can be updated
  display.clearDisplay();

  //text options
  display.setTextSize(1);
  display.setTextColor(1);
  //position of where text begins
  display.setCursor(0, 0);

  //update data and adds to display buffer
  display.print("RPM: ");
  display.println(rpm);
  
  //sends display buffer to display which causes the update
  display.display();
}