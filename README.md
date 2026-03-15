# Wind Energy Club
This respoestory stores the arduino projects I've done as a member of the software team for the wind energy club at Penn State. All of the clubs repository can be found here: [https://github.com/PSU-CWC](url)

## Linear Actuator Control System
This is an Arduino program that allows for real time control of our wind turbine's pitching mechanism. Our turbine has a pitching mechanism that is controled using a linear actuator. Originally the pitching mechanism was controlled with an ESP32 with the pitching angle hard coded in the software. This meant that during testing new code had to be flashed to the ESP32 everytime the Areo team needed to test a different angle which took upa majority of their testing time. They came to me with this issue and asked me to find a simple solution that would allow them to efficiently test a variety of pitching angles without stopping the test. I proposed this arduino program that uses an incremental encoder and an OLED display that controls the linear actuator and displays its position. This cut pitching angle testing time in half allowing our areo team to test more each weekend.

### Requirements  
Arduino IDE  
  
Additional Aduino Libraries:  
- Adafruit_GFX_Library  
- Adafruit_SH110X  

Additional libraries can be installed through the Aruindo IDE in the library panel on the left side.  
Both libraries and dependencies must be installed for code to compile.  

### Wiring/Pinouts
``` 
  OLED Display:
 
      SCK/SCL and SDA ports are sent in libraries used and cannot be changed in this file
      Some boards have SCK/SCL and SDA pins which will also work 

      SCk/SCL    - A5  
      SDA        - A4    
      VCC        - 5V  
      GND        - GND


  Actuator:
      white(signal) - pin 4 
      red(5V)       - 5V
      Black(GND)    - GND


  Incremental Encoder:

      three pins: A, GND, B
      two pins: switch, GND
          A             - 3
          GND           - GND
          B             - 5
          switch        - 7
```
### 
![Image](https://github.com/user-attachments/assets/22b88eee-f53f-4bfa-8bd0-c0465c28f807)

## RPM Sensor
This is a simple arduino program that allows our generator team to get real time RPM data of our generator during testing. Our generator team needed a simple way to measure the rpm of our generator. Previously, everytime our team needed to test our generator they had to boot up our control software and configure the settings. This was incredibly time consuming as the majority of time the team only needed rpm data. This simple system allowed our team to plug in the power and immediately get constant real time rpm data.

### Requirements  
Arduino IDE  
  
Additional Aduino Libraries:  
- Adafruit_GFX_Library  
- Adafruit_SH110X  

Additional libraries can be installed through the Aruindo IDE in the library panel on the left side.  
Both libraries and dependencies must be installed for code to compile.

### Wiring/Pinouts
