//TODO: add feedback for linear actuator

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Servo.h"

/*
PORTS: 
  Display:
    SCK/SCL and SDA ports are sent in libraries used and cannot be changed in this file
    SCk/SCL - A5, 
    SDA     - A4, 
    VCC           - 5V(must be 5V instead of 3.3), 
    GND           - GND
  Actuator:
    white(signal) - pin 4, 
    red(5V)       - 5V,
    Black(GND)    - GND     
  Encoder:
    three pins: A, GND, B
    two pins: switch, GND
    A             - 3,
    GND           - GND,
    B             - 5,
    switch        - 7,
*/


//incremental encoder variables
const int pinA = 3;
const int pinB = 5;
const int btn = 7;

volatile int encoderPos = 0;
int lastEncoded = 0;
bool processInput = true;

//actuator variables
const int RC_PIN = 4;
Servo actuator;




//OLED screen variables
int WHITE = 1;//pixel on
int BLACK = 0;//pixel off
int SCREEN_WIDTH = 128;  // OLED display width, in pixels
int SCREEN_HEIGHT = 64;  // OLED display height, in pixels
// Declaration for SSD1306 display connected using I2C
int SCREEN_ADDRESS = 0x3C;
//-1 default reset code for screen (same as arduino)
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//used to set linear actuator position
int currentPos = 0;
int setPos = 0;
const int MAX_POS = 100;


void setup() {
  Wire.begin();
  //Wire.setClock(100000);
  Serial.begin(9600);

  //pin set ups
  //A4 and A5 already set up through adafruit
  //encoder pins
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(btn, INPUT_PULLUP);
  //actuator pins
  actuator.attach(RC_PIN);

//actuator setup
  actuator.writeMicroseconds(1000);//sets to 0

//encoder setup
  //initial state for encoder
  lastEncoded = (digitalRead(pinA) << 1) | digitalRead(pinB);

//OLED setup
  // initialize the OLED object
  if (!display.begin(SCREEN_ADDRESS, true)) {
    Serial.println(F("SSD allocation failed"));
    while(true){}//loops forever if allocation of display fails 
  }
  display.clearDisplay();
  display.display();
}


unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_INTERVAL = 500; // ms

void loop() {
  
  //checks for encoder turned
  updateEncoder();
  
  //only updates display every DISPLAY_INTERVAL ms
  //updating display is blocking and takes long time 
  //which prevents encoder from accurately being read
  //so encoder is updated every loop while display updates every couple hundred loops
  unsigned long now = millis();//returns the number of miliseconds passed since the arduino started running
  if (now - lastDisplayUpdate >= DISPLAY_INTERVAL) {//
    lastDisplayUpdate = now;//
    updateDisplay();
  }
  
  //handles button opperations
  if (digitalRead(btn) == LOW) {
    btnPressed();
  }

  delay(2);
}


void updateDisplay(){
  //clears display buffer so data can be updated
  display.clearDisplay();

  //text options
  display.setTextSize(1);
  display.setTextColor(1);
  //position of where text begins
  display.setCursor(0, 0);

  //update data and adds to display buffer
  displayCurrentPos();//only updates when button is pressed
  display.println();
  displaySetPos();
  //will be added later
  //displayPositionBar();//updates when dial spins
  //displayGraphic
  
  //sends display buffer to display which causes the update
  display.display();
}

//
void displayCurrentPos(){
  display.println("Current Pos: ");
  display.print(currentPos);
  display.println("%");
  //display.setCursor(80,10);
  display.print(convertToInches(currentPos));
  display.println(" in");
}

void displaySetPos(){
  display.println("Set Pos: ");
  display.print(setPos);
  display.println("%");
  //display.setCursor(80,10);
  display.print(convertToInches(setPos));
  display.println(" in");
}

void btnPressed(){
  //actuator position updated
  Serial.print("encoderPos: ");
  Serial.println(encoderPos);

  Serial.print("Current: ");
  Serial.println(currentPos);
  Serial.print("Set: ");
  Serial.println(setPos);
  
  currentPos = setPos;
  
  //map 
  int rc = map(currentPos, 0, 100, 1000, 2000);
  //move actuator
  actuator.writeMicroseconds(rc);
  //encoder position reset
  //encoderPos = 0;
  delay(500);
}


void updateEncoder() {
  //encoder calculations
  int MSB = digitalRead(pinA);
  int LSB = digitalRead(pinB);

  int encoded = (MSB << 1) | LSB;          //two bits (MSB,LSB) which are A and B pins
  int sum = (lastEncoded << 2) | encoded;  // shifts last bits over to the left and adds two newest to the right
  
  //determines which way the encoder was turned based on the phase shift of the encoder
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    Serial.println("increase");
    encoderPos++;
    lastEncoded = encoded;
  } 
  else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    Serial.println("decrease");
    encoderPos--;
    lastEncoded = encoded;
  }
  //corrects for encoder increaseing by 4 each tick turned - could probably increase delay at end of loop() to fix this instead
  setPos = encoderPos>>2;
  if(setPos>MAX_POS){
    setPos = MAX_POS;
  }
  else if(setPos<0){
    setPos = 0;
  }
}


float convertToInches(int percent){
  //return float(percent)*1.2/100; inches
  return float(percent)*30.48/100;
}

//extra functions will be added later
//void displaySetPos(){
  //displays a bar across the screen for the current position and set position
//}
//void displayGraphic(){
  //displays a bar across the screen for the current position and set position
//}


//reads from the linear acctuator
//float getAcctuatorPos(){
//read function from servo
//}



