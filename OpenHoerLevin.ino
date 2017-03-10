/***********************************************************/
/*
    Open Source MP3 Player designed for kids
    Envoirement: DEV
*/

// Include Keypad Library
#include <Key.h>
#include <Keypad.h>


// Serial MP3 Player Catalex (YX5300 chip)
#define Serial3    // Connect the MP3 Serial Player to the Arduino MEGA Serial3 (14 TX3 -> RX, 15 RX3 -> TX)
#define SWITCH 8 // mp3addon Switch
#define DEBUG false // Debug 
#define ledPin 9 // Status Led

// ** Keypad
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'a'},
  {'4', '5', '6', 'b'},
  {'7', '8', '9', 'c'},
  {'d', 'p', 'u', 'd'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 9, 6, 4, 2 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 10, 8, 5, 3 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// ** End keypad

#include "mp3serial.h"  // Default Arduino MEGA Serial3. Change this file for Arduino UNO suport.
#include "mp3addon.h" // Addon for special features :-)

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(9600);
  myMP3.begin(9600);
  delay(500);
  sendCommand(CMD_SEL_DEV, DEV_TF);  //Select Device
  delay(200);
  sendCommand(CMD_SET_VOLUME, 0x0f); //Set Standard volume to 15
  digitalWrite(ledPin, HIGH);   // sets the LED on
}

void loop()
{
  char c = ' ';

  // If there a char on Serial call sendMP3Command to sendCommand
  if ( Serial.available() ) {
    c = Serial.read();
    sendMP3Command(c);
  }

  // Check for the answer.
  if (myMP3.available())
  {
    Serial.println("Answer: " + decodeMP3Answer());
  }

  // Check for key pressed from the keypad
  char key = kpd.getKey();
  if (key) // Check for a valid key.
  {
    Serial.println(key);
    sendMP3Command(key);
  }
  delay(200);
}

void debug(String msg) {
  if (DEBUG)
  {
    Serial.println(msg);
  }
}
