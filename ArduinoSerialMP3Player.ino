/***********************************************************/
/* 
 *  Open Source MP3 Player designed for kids
 *  Envoirement: DEV
 */
// Serial MP3 Player Catalex (YX5300 chip)
#define Serial3    // Connect the MP3 Serial Player to the Arduino MEGA Serial3 (14 TX3 -> RX, 15 RX3 -> TX)
#define SWITCH 8 // mp3addon Switch
#define SENSOR A1 // Poti for volume control
#define DEBUG true


#include "mp3serial.h"  // Default Arduino MEGA Serial3. Change this file for Arduino UNO suport.
#include "mp3addon.h" // Addon for special features :-)


void setup() 
{
  Serial.begin(9600);
  myMP3.begin(9600);
  //myfeature.begin(9600);
  delay(500);
    sendCommand(CMD_SEL_DEV, DEV_TF);  //Select Device
  delay(200);
    sendCommand(CMD_SET_VOLUME, 0x0f); //Set Standard volume to 15
}

void loop() 
{
 char c=' ';
  
  // If there a char on Serial call sendMP3Command to sendCommand
   if( Serial.available() )
    {
    c = Serial.read();
    sendMP3Command(c);
    }
    
  // Check for the answer.
  if (myMP3.available())
  {
    Serial.println("Answer: "+decodeMP3Answer());
  }

  //Run special features Switch LED on/off
  //if (!digitalRead(SWITCH))
  //{
  //   specialfeature(SWITCH);
  //}

  // Volume control
  Serial.println("Sensor=" + String(analogRead(SENSOR)));
  delay(200);
}

void debug(String msg){
  if (DEBUG)
  {
    Serial.println(msg);
  }
}
