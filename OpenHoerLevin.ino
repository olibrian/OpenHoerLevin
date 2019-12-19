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
#define ledPin 9 // Status Led

// #define SSID "[Your SSID]"
// #define PASSWORD "[Your Password]"


#define SSID "Mweg"
#define PASSWORD "Mattenweg16@Wabern"

#define LED 9
#define LED_WLAN 13

#define DEBUG true



const byte site[] PROGMEM = {
60,72,84,77,76,62,60,72,69,65,68,62,10,60,109,101,116,97,32,110,97,109,101,61,34,118,105,101,119,112,111,114,116,34,32,99,111,110,116,101,110,116,61,34,119,105,100,116,104,61,100,101,118,105,99,101,45,119,105,100,116,104,44,32,105,110,105,116,105,97,108,45,115,99,97,108,101,61,50,46,48,44,32,117,115,101,114,45,115,99,97,108,97,98,108,101,61,121,101,115,34,62,10,60,116,105,116,108,101,62,10,83,119,105,116,99,104,32,76,69,68,10,60,47,116,105,116,108,101,62,10,60,47,72,69,65,68,62,10,10,60,66,79,68,89,32,98,103,99,111,108,111,114,61,34,35,70,70,70,70,57,57,34,32,116,101,120,116,61,34,35,48,48,48,48,48,48,34,62,10,60,70,79,78,84,32,115,105,122,101,61,34,54,34,32,70,65,67,69,61,34,86,101,114,100,97,110,97,34,62,10,83,119,105,116,99,104,32,76,69,68,10,60,47,70,79,78,84,62,10,10,60,72,82,62,10,60,66,82,62,10,60,70,79,78,84,32,115,105,122,101,61,34,51,34,32,70,65,67,69,61,34,86,101,114,100,97,110,97,34,62,10,83,119,105,116,99,104,32,116,104,101,32,76,69,68,32,60,66,82,62,10,111,110,32,68,57,32,79,78,32,97,110,100,32,79,70,70,10,60,66,82,62,10,60,66,82,62,10,60,102,111,114,109,32,109,101,116,104,111,100,61,34,71,69,84,34,62,10,9,60,98,117,116,116,111,110,32,116,121,112,101,61,34,115,117,98,109,105,116,34,32,110,97,109,101,61,34,108,101,100,34,32,118,97,108,117,101,61,34,49,34,62,76,69,68,32,79,78,60,47,98,117,116,116,111,110,62,10,9,60,98,117,116,116,111,110,32,116,121,112,101,61,34,115,117,98,109,105,116,34,32,110,97,109,101,61,34,108,101,100,34,32,118,97,108,117,101,61,34,48,34,62,76,69,68,32,79,70,70,60,47,98,117,116,116,111,110,62,10,60,47,102,111,114,109,62,10,60,66,82,62,10,60,72,82,62,10,10,60,47,102,111,110,116,62,10,60,47,72,84,77,76,62
};

#include <SoftwareSerial.h>
SoftwareSerial esp8266(11, 10); // RX, TX


// ** Keypad
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'d', 'p', 'u'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 9, 6, 4, 2 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 8, 5, 3 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// ** End keypad

#include "mp3serial.h"  // Default Arduino MEGA Serial3. Change this file for Arduino UNO suport.

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(19200);
  myMP3.begin(19200);
  esp8266.begin(19200);
  delay(500);

  myMP3.listen();
  Serial.println("Select Device");
  sendCommand(CMD_SEL_DEV, DEV_TF);  //Select Device
  delay(200);
  Serial.println("Set Standard Volume to 15");
  sendCommand(CMD_SET_VOLUME, 0x0f); //Set Standard volume to 15
  digitalWrite(ledPin, HIGH);   // sets the LED on


  Serial.println("Start Webserver");
  esp8266.listen();
  if (!espConfig()) serialDebug();
  if (configTCPServer())  debug("Server Aktiv"); else debug("Server Error");   //Start TCP Server
}

void loop()
{
  myMP3.listen();
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

// Communication Webserver
  esp8266.listen();
  String xBuffer;
  if (esp8266.available()) // check if the esp is sending a message
  {
    if (esp8266.find("+IPD,"))
    {
      debug("Incomming Request");
      int connectionId = esp8266.parseInt();

      if (esp8266.findUntil("?led=", "\n")) digitalWrite(LED, esp8266.parseInt());
      if (sendWebsite(connectionId, createWebsite())) debug("Website send OK"); else debug("Website send Error");
    }
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
