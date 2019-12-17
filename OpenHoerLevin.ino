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
#define ledPin 9 // Status Led

#define SSID "[Your SSID]"
#define PASSWORD "[Your Password]"

#define LED 9
#define LED_WLAN 13

#define DEBUG true



const byte site[] PROGMEM = {
3c,48,54,4d,4c,3e,3c,48,45,41,44,3e,0a,3c,6d,65,74,61,20,6e,61,6d,65,3d,22,76,69,65,77,70,6f,72,74,22,20,63,6f,6e,74,65,6e,74,3d,22,77,69,64,74,68,3d,64,65,76,69,63,65,2d,77,69,64,74,68,2c,20,69,6e,69,74,69,61,6c,2d,73,63,61,6c,65,3d,32,2e,30,2c,20,75,73,65,72,2d,73,63,61,6c,61,62,6c,65,3d,79,65,73,22,3e,0a,3c,74,69,74,6c,65,3e,0a,53,77,69,74,63,68,20,4c,45,44,0a,3c,2f,74,69,74,6c,65,3e,0a,3c,2f,48,45,41,44,3e,0a,0a,3c,42,4f,44,59,20,62,67,63,6f,6c,6f,72,3d,22,23,30,30,30,30,46,46,22,20,74,65,78,74,3d,22,23,30,30,30,30,30,30,22,3e,0a,3c,46,4f,4e,54,20,73,69,7a,65,3d,22,36,22,20,46,41,43,45,3d,22,56,65,72,64,61,6e,61,22,3e,0a,4f,70,65,6e,20,48,6f,65,72,20,4c,65,76,69,6e,20,57,65,62,20,49,6e,74,65,72,66,61,63,65,0a,3c,2f,46,4f,4e,54,3e,0a,0a,3c,48,52,3e,0a,3c,42,52,3e,0a,3c,46,4f,4e,54,20,73,69,7a,65,3d,22,33,22,20,46,41,43,45,3d,22,56,65,72,64,61,6e,61,22,3e,0a,43,6f,6e,74,72,6f,6c,20,4f,48,4c,20,62,79,20,62,72,6f,77,73,65,72,20,3c,42,52,3e,0a,61,6e,64,20,67,65,74,20,74,68,65,20,63,6f,6e,74,72,6f,6c,20,62,61,63,6b,0a,3c,42,52,3e,0a,3c,42,52,3e,0a,3c,66,6f,72,6d,20,6d,65,74,68,6f,64,3d,22,47,45,54,22,3e,0a,09,3c,62,75,74,74,6f,6e,20,74,79,70,65,3d,22,73,75,62,6d,69,74,22,20,6e,61,6d,65,3d,22,6c,65,64,22,20,76,61,6c,75,65,3d,22,70,22,3e,50,6c,61,79,2f,50,61,75,73,65,3c,2f,62,75,74,74,6f,6e,3e,20,3c,42,52,3e,0a,09,3c,62,75,74,74,6f,6e,20,74,79,70,65,3d,22,73,75,62,6d,69,74,22,20,6e,61,6d,65,3d,22,6c,65,64,22,20,76,61,6c,75,65,3d,22,70,22,3e,50,72,65,76,3c,2f,62,75,74,74,6f,6e,3e,0a,09,3c,62,75,74,74,6f,6e,20,74,79,70,65,3d,22,73,75,62,6d,69,74,22,20,6e,61,6d,65,3d,22,6c,65,64,22,20,76,61,6c,75,65,3d,22,6e,22,3e,4e,65,78,74,3c,2f,62,75,74,74,6f,6e,3e,20,3c,42,52,3e,0a,09,3c,62,75,74,74,6f,6e,20,74,79,70,65,3d,22,73,75,62,6d,69,74,22,20,6e,61,6d,65,3d,22,6c,65,64,22,20,76,61,6c,75,65,3d,22,75,22,3e,56,6f,6c,75,6d,65,20,55,70,3c,2f,62,75,74,74,6f,6e,3e,0a,09,3c,62,75,74,74,6f,6e,20,74,79,70,65,3d,22,73,75,62,6d,69,74,22,20,6e,61,6d,65,3d,22,6c,65,64,22,20,76,61,6c,75,65,3d,22,64,22,3e,56,6f,6c,75,6d,65,20,44,6f,77,6e,3c,2f,62,75,74,74,6f,6e,3e,0a,0a,3c,2f,66,6f,72,6d,3e,0a,3c,42,52,3e,0a,3c,48,52,3e,0a,0a,3c,2f,66,6f,6e,74,3e,0a,3c,2f,48,54,4d,4c,3e,0a};

#include <SoftwareSerial.h>
SoftwareSerial esp8266(11, 12); // RX, TX


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

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(9600);
  myMP3.begin(9600);
  delay(500);
  Serial.println("Select Device");
  sendCommand(CMD_SEL_DEV, DEV_TF);  //Select Device
  delay(200);
  Serial.println("Set Standard Volume to 15");
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


//-----------------------------------------SendWebsite------------------------------------


boolean sendWebsite(int connectionId, String webpage)
{
  boolean succes = true;

  if (sendCom("AT+CIPSEND=" + String(connectionId) + "," + String(webpage.length()), ">"))
  {
    esp8266.print(webpage);
    esp8266.find("SEND OK");
    succes &= sendCom("AT+CIPCLOSE=" + String(connectionId), "OK");
  }
  else
  {
    succes = false;
  }
  return succes;
}

String createWebsite()
{
  String xBuffer;

  for (int i = 0; i <= sizeof(site); i++)
  {
    char myChar = pgm_read_byte_near(site + i);
    xBuffer += myChar;
  }

  return xBuffer;
}

//-----------------------------------------Config ESP8266------------------------------------

boolean espConfig()
{
  boolean succes = true;
  esp8266.setTimeout(5000);
  succes &= sendCom("AT+RST", "ready");
  esp8266.setTimeout(1000);
  if (configStation(SSID, PASSWORD)) {
    succes &= true;
    debug("WLAN Connected");
    debug("My IP is:");
    debug(sendCom("AT+CIFSR"));
  }
  else
  {
    succes &= false;
  }
  //shorter Timeout for faster wrong UPD-Comands handling
  succes &= sendCom("AT+CIPMODE=0", "OK");  //So rum scheit wichtig!
  succes &= sendCom("AT+CIPMUX=0", "OK");

  return succes;
}

boolean configTCPServer()
{
  boolean succes = true;

  succes &= (sendCom("AT+CIPMUX=1", "OK"));
  succes &= (sendCom("AT+CIPSERVER=1,80", "OK"));

  return succes;

}

boolean configTCPClient()
{
  boolean succes = true;

  succes &= (sendCom("AT+CIPMUX=0", "OK"));
  //succes &= (sendCom("AT+CIPSERVER=1,80", "OK"));

  return succes;

}


boolean configStation(String vSSID, String vPASSWORT)
{
  boolean succes = true;
  succes &= (sendCom("AT+CWMODE=1", "OK"));
  esp8266.setTimeout(20000);
  succes &= (sendCom("AT+CWJAP=\"" + String(vSSID) + "\",\"" + String(vPASSWORT) + "\"", "OK"));
  esp8266.setTimeout(1000);
  return succes;
}

boolean configAP()
{
  boolean succes = true;

  succes &= (sendCom("AT+CWMODE=2", "OK"));
  succes &= (sendCom("AT+CWSAP=\"NanoESP\",\"\",5,0", "OK"));

  return succes;
}

boolean configUDP()
{
  boolean succes = true;

  succes &= (sendCom("AT+CIPMODE=0", "OK"));
  succes &= (sendCom("AT+CIPMUX=0", "OK"));
  succes &= sendCom("AT+CIPSTART=\"UDP\",\"192.168.255.255\",90,91,2", "OK"); //Importand Boradcast...Reconnect IP
  return succes;
}

//-----------------------------------------------Controll ESP-----------------------------------------------------

boolean sendCom(String command, char respond[])
{
  esp8266.println(command);
  if (esp8266.findUntil(respond, "ERROR"))
  {
    return true;
  }
  else
  {
    debug("ESP SEND ERROR: " + command);
    return false;
  }
}

boolean sendUDP(String Msg)
{
  boolean succes = true;

  succes &= sendCom("AT+CIPSEND=" + String(Msg.length() + 2), ">");    //+",\"192.168.4.2\",90", ">");
  if (succes)
  {
    succes &= sendCom(Msg, "OK");
  }
  return succes;
}




String sendCom(String command)
{
  esp8266.println(command);
  return esp8266.readString();
}



//-------------------------------------------------Debug Functions------------------------------------------------------
void serialDebug() {
  while (true)
  {
    if (esp8266.available())
      Serial.write(esp8266.read());
    if (Serial.available())
      esp8266.write(Serial.read());
  }
}


void debug(String msg) {
  if (DEBUG)
  {
    Serial.println(msg);
  }
}
