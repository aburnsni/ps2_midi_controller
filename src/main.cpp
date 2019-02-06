/*
PS2Controlv0.ino
21 September 2015
Hamish Trolove - www.techmonkeybusiness.com

This sketch illustrates the use of a Playstation 2 Controller to
actuate a series of servos and an Electronic Speed Controller (ESC)
as you might do with a vehicle.  An LED light is used to illustrate
the use of the Playstation 2 Buttons.

Pin assignments are:

3.3V output to PS2 red pin
5V output to 1Kohm pull up resistors for PS2.
Pin D02 to PS2 brown pin (data)
Pin D03 to PS2 yellow pin (attention)
Pin D04 to PS2 orange pin (command)
Pin D05 to PS2 blue pin (clock)

Pin D06 to ESC Signal Pin  
Pin D07 to Steering Servo Signal pin
Pin D08 to Servo 1 Signal pin
Pin D09 to Servo 2 Signal pin

Pin D13 to LED Transistor Base

The ESC servo connection supplies 5.6V to the servos.

The coding pulls on the PS2X library developed by Bill Porter.
See www.billporter.info for the latest from Bill Porter and to
download the library.

The controls used for this sketch are;
Right Stick - X-axis = Steering Servo left/right, Y-axis = ESC forward/backward
Left Stick - X-axis = Servo 2 left/right, Y-axis = Servo 1 left/right

Triangle = Toggle the LED
*/

#include <Arduino.h>
#include <PS2X_lib.h> // Bill Porter's PS2 Library
#include <MIDI.h>
#include "drums.h"
#include "patch.h"

PS2X ps2x;  //The PS2 Controller Class
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

#define pressures   true
#define rumble      true
int channel = 10;

int error = 0;
byte type = 0;
const int LEDpin = 13;  //green LED is on Digital pin 13
const bool DEBUG = 0;

int PlyStnRStickUpDn = 0;  //Value read off the PS2 Right Stick up/down.
int PlyStnRStickLtRt = 0;  //Value read off the PS2 Right Stick left/right
int PlyStnLStickUpDn = 0; //Value read off the PS2 Left Stick up/down
int PlyStnLStickLtRt = 0; // Value read off the PS2 Left Stick left/right

void playNote(int note, int vol) {
  MIDI.sendNoteOn(note, vol, channel);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5);
  MIDI.sendNoteOff(note,100,channel);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup()
{
  MIDI.begin();
  if (DEBUG) {
    Serial.begin(115200);
  }

  error = ps2x.config_gamepad(5,4,3,2, pressures, rumble);
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures, Rumble)
  if (DEBUG) {
    if(error == 0){
      Serial.print("Found Controller, configured successful ");
      Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
      Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    }  
    else if(error == 1)
      Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
    
    else if(error == 2)
      Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if(error == 3)
      Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
    //  Serial.print(ps2x.Analog(1), HEX);
  }    
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      if (DEBUG)
        Serial.print("Unknown Controller type found ");
      break;
    case 1:
      if (DEBUG)
        Serial.print("DualShock Controller found ");
      break;
    case 2:
      if (DEBUG)
        Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      if (DEBUG)
        Serial.print("Wireless Sony DualShock Controller found ");
      break;
  }
  pinMode(LEDpin, OUTPUT);  //Sets the LEDpin to output
}

void loop()
{
  ps2x.read_gamepad(); //This needs to be called at least once a second
                        // to get data from the controller.

  // if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
  //   Serial.print("Stick Values:");
  //   Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
  //   Serial.print(",");
  //   Serial.print(ps2x.Analog(PSS_LX), DEC); 
  //   Serial.print(",");
  //   Serial.print(ps2x.Analog(PSS_RY), DEC); 
  //   Serial.print(",");
  //   Serial.println(ps2x.Analog(PSS_RX), DEC); 
  // }  

  // if (ps2x.NewButtonState()) {
      if(ps2x.ButtonPressed(PSB_L1))
        playNote(ltrig1, 100);
      if(ps2x.ButtonPressed(PSB_R1))
        playNote(rtrig1, 100);
      if(ps2x.ButtonPressed(PSB_L2))
        playNote(ltrig2, 100);
      if(ps2x.ButtonPressed(PSB_R2))
        playNote(rtrig2, 100);
      if(ps2x.ButtonPressed(PSB_TRIANGLE))
        playNote(button_triangle, 100);
      if(ps2x.ButtonPressed(PSB_CIRCLE))
        playNote(button_circle, 100);
      if(ps2x.ButtonPressed(PSB_CROSS))
        playNote(button_cross, 100); 
      if(ps2x.ButtonPressed(PSB_SQUARE))
        playNote(button_square, 100);
      // if(ps2x.Button(PSB_START))
      //   Serial.println("Start is being held");
      // if(ps2x.Button(PSB_SELECT))
      //   Serial.println("Select is being held");      
      if(ps2x.ButtonPressed(PSB_PAD_UP))
        playNote(dpad_up, ps2x.Analog(PSAB_PAD_UP)/2);
      if(ps2x.ButtonPressed(PSB_PAD_RIGHT))
        playNote(dpad_right, ps2x.Analog(PSAB_PAD_RIGHT)/2);
      if(ps2x.ButtonPressed(PSB_PAD_LEFT))
        playNote(dpad_left, ps2x.Analog(PSAB_PAD_LEFT)/2);
      if(ps2x.ButtonPressed(PSB_PAD_DOWN)) {
        playNote(dpad_down, ps2x.Analog(PSAB_PAD_DOWN)/2);
      }
  // }

  delay(5);
}